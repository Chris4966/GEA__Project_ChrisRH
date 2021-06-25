#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QMessageBox> //Help message
#include <QTreeWidgetItem> //Scene Outliner
#include <QDebug>
#include <QScreen>  //for resizing the program at start
#include <QString>
#include "visualobject.h"
#include "widgettransform.h"
#include "widgetmesh.h"
#include "widgetmaterial.h"
#include "widgetsound.h"
#include "resourcemanager.h"
#include "renderwindow.h"
#include "scene.h"
#include "coreengine.h"
#include "ecs/entity.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    //this sets up what's in the mainwindow.ui
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete ui;
}

void MainWindow::init()
{
    //This will contain the setup of the OpenGL surface we will render into
    QSurfaceFormat format;

    //OpenGL v 4.1 - (Ole Flatens Mac does not support higher than this - sorry!)
    //you can try other versions, but then have to update RenderWindow and Shader
    //to inherit from other than QOpenGLFunctions_4_1_Core
    format.setVersion(4, 1);
    //Using the main profile for OpenGL - no legacy code permitted
    format.setProfile(QSurfaceFormat::CoreProfile);
    //A QSurface can be other types than OpenGL
    format.setRenderableType(QSurfaceFormat::OpenGL);

    //This should activate OpenGL debug Context used in RenderWindow::startOpenGLDebugger().
    //This line (and the startOpenGLDebugger() and checkForGLerrors() in RenderWindow class)
    //can be deleted, but it is nice to have some OpenGL debug info!
    format.setOption(QSurfaceFormat::DebugContext);

    // The renderer will need a depth buffer - (not requiered to set in glfw-tutorials)
    format.setDepthBufferSize(24);

    //Set the number of samples used for multisampling
    //Same as glfwWindowHint(GLFW_SAMPLES, 4);
    //https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing
    format.setSamples(4);

    //Just prints out what OpenGL format we try to get
    // - this can be deleted
    qDebug() << "Requesting surface format: " << format;

    //We have a format for the OpenGL window, so let's make it:
    mRenderWindow = new RenderWindow(format, this);

    //Check if renderwindow did initialize, else prints error and quit
    if (!mRenderWindow->context()) {
        qDebug() << "Failed to create context. Can not continue. Quits application!";
        delete mRenderWindow;
        return;
    }

    //The OpenGL RenderWindow got made, so continuing the setup:
    //We put the RenderWindow inside a QWidget so we can put in into a
    //layout that is made in the .ui-file
    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindow,this);
    mRenderWindowContainer->setFocusPolicy(Qt::StrongFocus);
    //OpenGLLayout is made in the .ui-file!
    ui->OpenGLLayout->addWidget(mRenderWindowContainer);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    // mRenderWindowContainer->setFocus(); //moved to RenderWindow::init()

    //Set the size of the program in % of the actual screen size
    QSize tempSize = QGuiApplication::primaryScreen()->size();
    if (tempSize.width() / tempSize.height() > 1.8)     // if ultra wide
    {
        tempSize.rheight() *= 0.85;
        tempSize.rwidth() *= 0.55;
    }
    else                                                // normal aspect ratio
        tempSize *= 0.7;
    resize(tempSize);

    mCoreEngine = new CoreEngine();
}

void MainWindow::updateUI(const std::vector<Entity> &GameObjectData)
{
    ui->treeWidgetSceneOutliner->clear();

    // Create the new tree root
    QTreeWidgetItem* root = new QTreeWidgetItem(ui->treeWidgetSceneOutliner);
    root->setText(0,  QString::fromStdString(mRenderWindow->mScene1->mName));
    ui->treeWidgetSceneOutliner->addTopLevelItem(root);
    ui->treeWidgetSceneOutliner->expandAll();

    for(unsigned i = 0; i < GameObjectData.size(); ++i)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(root);
        item->setText(0, QString::fromStdString(GameObjectData.at(i).mName));
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
}

void MainWindow::selectObjectByName(QString nameIn)
{
    if(nameIn == "")
        on_treeWidgetSceneOutliner_itemClicked(nullptr,0);
    else
    {
        QList<QTreeWidgetItem *> list = ui->treeWidgetSceneOutliner->findItems(nameIn, Qt::MatchFixedString|Qt::MatchRecursive);
        on_treeWidgetSceneOutliner_itemClicked(list.first(),0);
    }
}

void MainWindow::on_actionGrid_toggled(bool arg)
{
    mRenderWindow->mRenderEditorGrid = arg;
}

void MainWindow::on_actionAxis_toggled(bool arg)
{
    mRenderWindow->mRenderEditorAxis = arg;
}

void MainWindow::on_actionCorner_Gizmo_toggled(bool arg)
{
    mRenderWindow->mRenderCornerGizmo = arg;
}

void MainWindow::on_actionWireframe_toggled(bool arg)
{
    mRenderWindow->toggleWireframe(arg);
}

void MainWindow::on_actionDraw_Normals_toggled(bool arg)
{
    mRenderWindow->mDrawNormals = arg;
}

void MainWindow::on_actionBasic_Controls_triggered()
{
    QMessageBox::about(this, "Basic Controls",
                       "Move and rotate camera by holding down right mouse button and"
                       "\nuse WASD. Q = down, E = up. "
                       "\nUse ScrollWheel and Left Shift to adjust speed. "
                       "\nPlayer movement = WASD."
                       "\nPick up / drop object = E."
                       "\nToggle Player Camera = TAB."
                       "\nToggle Tips = X."
                       "\nRestart Game = R");
}

void MainWindow::on_treeWidgetSceneOutliner_itemClicked(QTreeWidgetItem *item, int )
{
    clearLayout(ui->verticalLayoutDetails); //delete all widgets in the details panel

    if (!item || item->text(0) == mRenderWindow->mScene1->mName.c_str())   //Top node selected or no selection
    {
        mRenderWindow->cancelPickedObject();
        ui->lineEditGOName->setText("no selection");
        return;
    }
    mCurrentEditItem = item;
    ui->lineEditGOName->setText(mCurrentEditItem->text(0));
    item->setSelected(true);

    //Transform widget:
    mTransformWidget = new WidgetTransform(this);
    mTransformWidget->setObjectName("TransformWidget");
    ui->verticalLayoutDetails->addWidget(mTransformWidget);
    mCurrentEditItemIndex = item->parent()->indexOfChild(item);
    qDebug() <<"Index" << mCurrentEditItemIndex;
    mTransformWidget->indexInSceneArray = mCurrentEditItemIndex;
    mTransformWidget->mCurrentScene = mRenderWindow->mScene1;
    mTransformWidget->readPosition();
    mTransformWidget->readRotation();
    mTransformWidget->readScale();
    mTransformWidget->mPositionStep = mPositionStep;
    mTransformWidget->mRotationStep = mRotationStep;
    mTransformWidget->mScaleStep = mScaleStep;
    mTransformWidget->clearFocus();

    //Mesh widget:
    mMeshWidget = new WidgetMesh(this);
    mMeshWidget->setObjectName("MeshWidget");
    ui->verticalLayoutDetails->addWidget(mMeshWidget);
    mMeshWidget->indexInSceneArray = mCurrentEditItemIndex;
    mMeshWidget->mCurrentScene = mRenderWindow->mScene1;
    mMeshWidget->readMeshData();

    //Material widget:
    mMaterialWidget = new WidgetMaterial(this);
    mMaterialWidget->setObjectName("MaterialWidget");
    ui->verticalLayoutDetails->addWidget(mMaterialWidget);
    mMaterialWidget->indexInSceneArray = mCurrentEditItemIndex;
    mMaterialWidget->mCurrentScene = mRenderWindow->mScene1;
    mMaterialWidget->readMaterialData();

    //Sound widget:
    mSoundWidget = new WidgetSound(this);
    mSoundWidget->setObjectName("SoundWidget");
    ui->verticalLayoutDetails->addWidget(mSoundWidget);
    mSoundWidget->indexInSceneArray = mCurrentEditItemIndex;
    mSoundWidget->mCurrentScene = mRenderWindow->mScene1;
    mSoundWidget->readSoundData();

    mRenderWindow->setPickedObject(mCurrentEditItemIndex);
//    mRenderWindowContainer->activateWindow(); // so input is at the renderWindow
    mRenderWindowContainer->setFocus();
//    mRenderWindowContainer->grabKeyboard();
}

void MainWindow::clearLayout(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->widget()) {   //probably not neccesary
            delete item->widget();
        }
        delete item; //probably not neccesary - Qt should do it automatically
    }
    mTransformWidget = nullptr;
    ui->treeWidgetSceneOutliner->clearSelection();
}

void MainWindow::on_actionPlay_toggled(bool arg1)
{
    isRunning = arg1;
    qDebug() << isRunning;
    mCoreEngine->bTogglePlay = isRunning;
    if (!isRunning)
    {
        mRenderWindow->mScene1->reset();
        ResourceManager::getInstance().stopAllSound();
    }
    else
    {
        for (MAX_ENTITIES_TYPE i{0}; i < mRenderWindow->mScene1->mEntities.size(); i++)
        {
            mRenderWindow->mScene1->mEntities[i].mInitPos = mRenderWindow->mScene1->mEntities[i].mTransform.position;
        }

        if (mRenderWindow->bToggleSound)
        {
            ResourceManager::getInstance().playSound(mRenderWindow->mPlayerID);
            ResourceManager::getInstance().playSound(mRenderWindow->mScene1->mEnemyIDs[0]);
            ResourceManager::getInstance().playSound(mRenderWindow->mScene1->mEnemyIDs[1]);
        }
    }
}

void MainWindow::on_actionRender_MousePick_toggled(bool arg1)
{
    renderMousePick = arg1;
}

void MainWindow::on_lineEditGOName_returnPressed()
{
//    qDebug() << ui->lineEditGOName->text();
    mCurrentEditItem->setText(0, ui->lineEditGOName->text());
//    mRenderWindow->mScene1->mVisualObjects.at(mCurrentEditItemIndex)->mName = ui->lineEditGOName->text().toStdString();
}

void MainWindow::on_positionStep_valueChanged(double arg1)
{
    mPositionStep = arg1;
    if(mTransformWidget)
    {
        mTransformWidget->mPositionStep = arg1;
        mTransformWidget->setStepRates();
    }
}

void MainWindow::on_rotationStep_valueChanged(double arg1)
{
    mRotationStep = arg1;
    if(mTransformWidget)
    {
        mTransformWidget->mRotationStep = arg1;
        mTransformWidget->setStepRates();
    }
}

void MainWindow::on_scaleStep_valueChanged(double arg1)
{
    mScaleStep = arg1;
    if(mTransformWidget)
    {
        mTransformWidget->mScaleStep = arg1;
        mTransformWidget->setStepRates();
    }
}

void MainWindow::on_actionTriangle_triggered()
{
    mRenderWindow->spawnEntity("triangle");
}

void MainWindow::on_actionQuad_triggered()
{
    mRenderWindow->spawnEntity("plane");
}


void MainWindow::on_actionCube_triggered()
{
    mRenderWindow->spawnEntity("box.obj");
}

void MainWindow::on_actionSphere_triggered()
{
    mRenderWindow->spawnEntity("ball.obj");
}

void MainWindow::on_actionFrustum_Culling_toggled(bool arg1)
{
    mRenderWindow->bToggleFrustum = arg1;
}

void MainWindow::on_actionLOD_toggled(bool arg1)
{
    mRenderWindow->bToggleLOD = arg1;
    if (!mRenderWindow->bToggleLOD)
    {
        ResourceManager::getInstance().setMesh(mRenderWindow->mPlayerID, "pawn_hd.obj");
        ResourceManager::getInstance().setMesh(mRenderWindow->mScene1->mEnemyIDs[0], "pawn_hd.obj");
        ResourceManager::getInstance().setMesh(mRenderWindow->mScene1->mEnemyIDs[1], "pawn_hd.obj");
    }
}

void MainWindow::on_actionSound_triggered(bool checked)
{
    mRenderWindow->bToggleSound = checked;
    if (mRenderWindow->bToggleSound && ResourceManager::getInstance().mCoreEngine->bTogglePlay)
    {
        ResourceManager::getInstance().playSound(mRenderWindow->mPlayerID);
        ResourceManager::getInstance().playSound(mRenderWindow->mScene1->mEnemyIDs[0]);
        ResourceManager::getInstance().playSound(mRenderWindow->mScene1->mEnemyIDs[1]);
    }
    else if (!mRenderWindow->bToggleSound) ResourceManager::getInstance().stopAllSound();
}


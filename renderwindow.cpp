#include "renderwindow.h"
#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <QGuiApplication>
#include <QPoint>
#include <QCursor>

#include <iostream>
#include <cmath>
#include <cassert>

//General stuff
#include "mainwindow.h"
#include "visualobject.h"
#include "camera.h"
#include "texture.h"
#include "gslpch.h"
#include "widgettransform.h"
#include "resourcemanager.h"
#include "coreengine.h"
#include "rendersystem.h"
#include "scriptsystem.h"
#include "Sound/soundmanager.h"
#include "Sound/soundsrc.h"

//scene / objects
#include "scene.h"
#include "light.h"
#include "xyz.h"
#include "editorgrid.h"
#include "gizmo.h"
#include "linebox.h"

//Shaders
#include "shader.h"
#include "phongshader.h"
#include "textureshader.h"
#include "colorshader.h"
#include "normalsshader.h"
#include "skyboxshader.h"
#include "mousepickershader.h"
#include "singlecolorshader.h"


RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)
{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);
}

// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        qDebug() << "makeCurrent() failed";
        return;
    }

    //    //just to make sure we don't init several times
    //    //used in exposeEvent()
    //    if (!mInitialized)
    //        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Print render version info (what GPU is used):
    //(Have to use cout to see text- qDebug just writes numbers...)
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    std::cout << "The active GPU and API: \n";
    std::cout << "  Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "  Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "  Version: " << glGetString(GL_VERSION) << std::endl;
    int textureUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnits);
    std::cout << "This GPU as " << textureUnits << " texture units / slots in total, ";
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &mTextureUnits);
    std::cout << "and supports " << mTextureUnits << " texture units pr shader" << std::endl;

    Q_ASSERT(gsl::NumberOfTextures < mTextureUnits && "Fix more flexible, robust system");

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);                //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);                 //draws only front side of models - usually what you want - test it out!
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);    // color used in glClear GL_COLOR_BUFFER_BIT

    //set up alpha blending for textures
    glEnable(GL_BLEND);// you enable blending function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //used for object outlining
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

    //Compile shaders and set up materials:
    shaderAndMaterialSetup();
    //init sound system
    ResourceManager::getInstance().initSound();

    //********************* Making the editor to be drawn **********************

    mIdentityMatrix = new gsl::mat4();
    mIdentityMatrix->identity();
    VisualObject *temp = new Gizmo();                           // 0
    temp->init();
    temp->mMaterial= ResourceManager::getInstance().mMaterial[5];
    mEditorAssets[gsl::GIZMO] = temp;

    temp = new EditorGrid(20, 1);                               // 1
    temp->init();
    temp->mMaterial= ResourceManager::getInstance().mMaterial[0];
    mEditorAssets[gsl::GRID] = temp;

    temp = new XYZ();                                           // 2
    temp->init();
    temp->mMaterial= ResourceManager::getInstance().mMaterial[0];
    mEditorAssets[gsl::AXIS] = temp;

    //********************** Set up scene **********************
    mMainWindow->mCoreEngine->mScenes.emplace_back(new Scene(this, mMainWindow->mCoreEngine->mRenderSystem));
    mScene1 = mMainWindow->mCoreEngine->mScenes.back();
    mMainWindow->mCoreEngine->mCurrentScene = mScene1;
    ResourceManager::getInstance().mCurrentScene = mScene1;
    mScene1->mSceneNr = 1;
    mScene1->init();
    mScene1->mName = "Scene A";

    //identifies certain entities as surface, player and camera mesh
    mSurfaceID = mScene1->mSurfaceID;
    mPlayerID = mScene1->mPlayerID;
    mCamID = mScene1->mCameraID;
    mLightID = mScene1->mLightID;

    //reads script data
    for(MAX_ENTITIES_TYPE i{0}; i<mScene1->mEntities.size(); i++)
    {
        if (mScene1->mEntities[i].bHasScript) ResourceManager::getInstance().mCoreEngine->mScriptSystem->setData(i, mScene1->mEntities[i].mScript.mName);
    }

    //********************** Set up camera **********************
    mCurrentCamera = new Camera();
    mCurrentCamera->setPosition(gsl::vec3(10.f, 10.f, 10.f));
    mCurrentCamera->pitch(50.f);
    mCurrentCamera->yaw(-45.f);


    //Update Scene Outliner
    mMainWindow->updateUI(mScene1->mEntities);

    //Put camera only in RenderSystem?
    //ColorShader needs camera
    static_cast<ColorShader*>(ResourceManager::getInstance().mShaderProgram[gsl::COLORSHADER])->mCurrentCamera = mCurrentCamera;
    //TextureShader needs camera
    static_cast<TextureShader*>(ResourceManager::getInstance().mShaderProgram[gsl::TEXTURESHADER])->mCurrentCamera = mCurrentCamera;
    //Phongshader needs light and camera:
    static_cast<PhongShader*>(ResourceManager::getInstance().mShaderProgram[gsl::PHONGSHADER])->mCurrentLight = &mScene1->mEntities[mLightID].mLight;
    static_cast<PhongShader*>(ResourceManager::getInstance().mShaderProgram[gsl::PHONGSHADER])->mCurrentCamera = mCurrentCamera;
    //Skybox needs camera
    static_cast<SkyboxShader*>(ResourceManager::getInstance().mShaderProgram[gsl::SKYBOXSHADER])->mCurrentCamera = mCurrentCamera;
    //MousePicker needs camera
    static_cast<MousePickerShader*>(ResourceManager::getInstance().mShaderProgram[gsl::MOUSEPICKERSHADER])->mCurrentCamera = mCurrentCamera;
    //SingleColor needs camera
    static_cast<SingleColorShader*>(ResourceManager::getInstance().mShaderProgram[gsl::SINGLECOLORSHADER])->mCurrentCamera = mCurrentCamera;

    setFocus();

    //TODO: move this to a better init place
    mMainWindow->mCoreEngine->mRenderSystem->init();
    ResourceManager::getInstance().mCoreEngine = mMainWindow->mCoreEngine;
    mScene1->prepareComponentArrays();
    ResourceManager::getInstance().stopAllSound();

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;
}

// Called each frame - this is actually the game loop!
void RenderWindow::render()
{
    //Keyboard / mouse input
    handleInput();

    mCurrentCamera->update();
    ResourceManager::getInstance().updateListener(mCurrentCamera->position(), {0.f, 0.f, 0.f}, mCurrentCamera->mForward, mCurrentCamera->mUp);
    mScene1->update();
    if (bPlayerCam) mScene1->mEntities[mCamID].mTransform.position = mScene1->mEntities[mPlayerID].mTransform.position + gsl::vec3{5.f, 10.f, 0.f};

    if (ResourceManager::getInstance().mCoreEngine->bTogglePlay)
    {
        //moves camera to playcam entity
        mCurrentCamera->setPosition(mScene1->mEntities[mCamID].mTransform.position);
        mCurrentCamera->setRotation(mScene1->mEntities[mCamID].mTransform.rotation);

        //checks if all entities are within the frustum and hides entities that are outside of it
        if (bToggleFrustum)
        {
            for (MAX_ENTITIES_TYPE i{1}; i < mScene1->mEntities.size(); i++)
            {
                if (!mScene1->mEntities[i].bIgnoreFrustum)
                {
                    if (mScene1->mEntities[i].bCollidable && !mCurrentCamera->frustumCull(mScene1->mEntities[i].mCollider, mScene1->mEntities[i].mTransform.scale))
                        mScene1->mEntities[i].bHidden = true;
                    else if (!mCurrentCamera->frustumCull(mScene1->mEntities[i].mTransform.position))
                        mScene1->mEntities[i].bHidden = true;
                    else mScene1->mEntities[i].bHidden = false;
                }
            }
        }

    }

    //hides invisible objects in play mode
    for (MAX_ENTITIES_TYPE i{0}; i < mScene1->mEntities.size(); i++)
    {
        for (unsigned short j{0}; j < mScene1->mEntities[i].mTags.size(); j++)
        {
            if (mScene1->mEntities[i].mTags[j] == "invisible")
                mScene1->mEntities[i].bHidden = ResourceManager::getInstance().mCoreEngine->bTogglePlay;
            if (mScene1->mEntities[i].mTags[j] == "hud")
                mScene1->mEntities[i].bHidden = !ResourceManager::getInstance().mCoreEngine->bTogglePlay;
        }
    }


    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)
    initializeOpenGLFunctions();    //must call this every frame it seems...

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glStencilMask(0x00); // make sure we don't update the stencil buffer while drawing the floor

    if(!mMainWindow->renderMousePick)
    {
        if (!mMainWindow->mCoreEngine->bTogglePlay) renderEditorAssets();
        mScene1->render();
    }
    else
        mousePicking();

    if(mDrawNormals)
    {
        glUseProgram(ResourceManager::getInstance().mShaderProgram[gsl::NORMALSSHADER]->mProgram);
        glUniformMatrix4fv( ResourceManager::getInstance().mShaderProgram[gsl::NORMALSSHADER]->vMatrixUniform, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
        glUniformMatrix4fv( ResourceManager::getInstance().mShaderProgram[gsl::NORMALSSHADER]->pMatrixUniform, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
        mScene1->renderNormals(ResourceManager::getInstance().mShaderProgram[gsl::NORMALSSHADER]->mMatrixUniform);
    }

    if(mPickedObject && mPickedObjectMarker)
    {
        glUseProgram(ResourceManager::getInstance().mShaderProgram[gsl::COLORSHADER]->mProgram);
        glUniformMatrix4fv( ResourceManager::getInstance().mShaderProgram[gsl::COLORSHADER]->vMatrixUniform, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
        glUniformMatrix4fv( ResourceManager::getInstance().mShaderProgram[gsl::COLORSHADER]->pMatrixUniform, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
        mPickedObjectMarker->draw();
        mMainWindow->mTransformWidget->readPosition();
    }
    glUseProgram(0);        //Nvidia card gives "shader in program ... recompiled based on GL state" warning without this

    //Calculate framerate before
    // checkForGLerrors() because that takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    //using our expanded OpenGL debugger to check if everything is OK.
    checkForGLerrors();

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.

    mContext->swapBuffers(this);
}

void RenderWindow::renderEditorAssets()
{
    glUseProgram(ResourceManager::getInstance().mShaderProgram[gsl::COLORSHADER]->mProgram);     // all editor assets uses color shder

    glUniformMatrix4fv( ResourceManager::getInstance().mShaderProgram[gsl::COLORSHADER]->vMatrixUniform, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
    glUniformMatrix4fv( ResourceManager::getInstance().mShaderProgram[gsl::COLORSHADER]->pMatrixUniform, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());

    if(mRenderEditorGrid)
    {
        glUniformMatrix4fv( ResourceManager::getInstance().mShaderProgram[gsl::COLORSHADER]->mMatrixUniform, 1, GL_TRUE, mEditorAssets[1]->mMatrix.constData());
        mEditorAssets[gsl::GRID]->draw();  //grid
    }
    if(mRenderEditorAxis)
    {
        glUniformMatrix4fv( ResourceManager::getInstance().mShaderProgram[gsl::COLORSHADER]->mMatrixUniform, 1, GL_TRUE, mEditorAssets[2]->mMatrix.constData());
        mEditorAssets[gsl::AXIS]->draw();  //XYZ
    }
    //Corner Gizmo
    if (mRenderCornerGizmo)
    {
        mEditorAssets[gsl::GIZMO]->mMatrix.setToIdentity();
        mEditorAssets[gsl::GIZMO]->mMatrix.rotateX(mCurrentCamera->mPitch);
        mEditorAssets[gsl::GIZMO]->mMatrix.rotateY(mCurrentCamera->mYaw);
        mEditorAssets[gsl::GIZMO]->mMatrix.setPosition(-0.88f, -0.88f, 0.0f);
        mEditorAssets[gsl::GIZMO]->draw();  //gizmo
    }
}

void RenderWindow::setPickedObject(int pickedID)
{
    cancelPickedObject();
    mPickedObject = &mScene1->mEntities[pickedID];

    mPickedObjectMarker = new LineBox(mPickedObject->mTransform.position - mPickedObject->mTransform.scale, mPickedObject->mTransform.position + mPickedObject->mTransform.scale);
    mPickedObjectMarker->mMaterial = ResourceManager::getInstance().mMaterial[0];

    mPickedObjectMarker->mMatrix.translateY(mPickedObject->mTransform.scale.y);
}

void RenderWindow::cancelPickedObject()
{
    mPickedObject = nullptr;
    if(mPickedObjectMarker)
    {
        delete mPickedObjectMarker;
        mPickedObjectMarker = nullptr;
    }
}

void RenderWindow::setFocus()
{
    mMainWindow->mRenderWindowContainer->setFocus();
}

void RenderWindow::mousePicking()
{
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)
    initializeOpenGLFunctions();    //must call this every frame it seems...
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    gsl::mat4 temp;
    static_cast<MousePickerShader*>(ResourceManager::getInstance().mShaderProgram[gsl::MOUSEPICKERSHADER])->use(temp, nullptr);
    checkForGLerrors();

    mScene1->renderMousepicker(ResourceManager::getInstance().mShaderProgram[gsl::MOUSEPICKERSHADER]->mMatrixUniform,
            static_cast<MousePickerShader*>(ResourceManager::getInstance().mShaderProgram[gsl::MOUSEPICKERSHADER])->mPickerColor);
    //    checkForGLerrors();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  //configure how glReadPixels will behave with respect to memory alignment
    //    checkForGLerrors();

    glFlush();  //Force all GL-commands to be sent
    glFinish(); //Force GL to finish rendering

    //    checkForGLerrors();

    // Read the color of the pixel at the mouse cursor.
    // Ultra-mega-over slow, even for 1 pixel,
    // because the framebuffer is on the GPU.
    unsigned char data[4];

    QPoint cursorPosition   = this->mapFromGlobal(this->cursor().pos());    //global mouse position converted to local
    //    qDebug() << "pos" << cursorPosition;

    glReadPixels(cursorPosition.x()*mRetinaScale, (height()- cursorPosition.y())*mRetinaScale , 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //    qDebug() << "Height" << height() * mRetinaScale << "Height - y" << (height()- cursorPosition.y())*mRetinaScale;
    int pickedID =
            data[0] +
            data[1] * 256 +
            data[2] * 256*256;

    pickedID = pickedID/20;   //  divide by 20 because of hack in Scene::renderMousePicker()

    if (pickedID < 100000)
    {
        qDebug() << "Mesh ID" << pickedID;
        mMainWindow->selectObjectByName(mScene1->mEntities.at(pickedID).mName.c_str());
        setPickedObject(pickedID);
    }
    else
    {
        mMainWindow->selectObjectByName("");
        cancelPickedObject();
    }

    setFocus();
}

void RenderWindow::shaderAndMaterialSetup()
{
    ResourceManager &tempRes = ResourceManager::getInstance();

    tempRes.readShaders(this);
    tempRes.readTextures();
    tempRes.makeMaterials();

    tempRes.readObjs(); ///Read all objs in asset folder!
}

void RenderWindow::setCameraSpeed(float value)
{
    mCameraSpeed += value;

    //Keep within some min and max values
    if(mCameraSpeed < 0.01f)
        mCameraSpeed = 0.01f;
    if (mCameraSpeed > 0.5f)
        mCameraSpeed = 0.5f;
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    mRetinaScale = devicePixelRatio();

    //Set viewport width and height
    glViewport(0, 0, static_cast<GLint>(width() * mRetinaScale), static_cast<GLint>(height() * mRetinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }

    //calculate aspect ration and set projection matrix
    mAspectratio = static_cast<float>(width()) / height();
    //    qDebug() << mAspectratio;
    mCurrentCamera->mProjectionMatrix.perspective(45.f, mAspectratio, 0.1f, 400.f);
    //    qDebug() << mCamera.mProjectionMatrix;
}

//The way this is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7) +
                                                  " | Camera Position: X: " + QString::number(mCurrentCamera->mPosition.x, 'g', 4) +
                                                  " Y: " + QString::number(mCurrentCamera->mPosition.y, 'g', 4) +
                                                  " Z: " + QString::number(mCurrentCamera->mPosition.z, 'g', 4));
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}

//Simple way to turn on/off wireframe mode
//Not totally accurate, but draws the objects with
//lines instead of filled polygons
void RenderWindow::toggleWireframe(bool buttonState)
{
    if (buttonState)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //turn on wireframe mode
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //turn off wireframe mode
        glEnable(GL_CULL_FACE);
    }
}

void RenderWindow::spawnEntity(std::string type)
{
    mScene1->addEntity(type, "Outline");
    mMainWindow->updateUI(mScene1->mEntities);
    mScene1->prepareComponentArrays();
}

void RenderWindow::reset()
{
    //restarts the game
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            qDebug() << "glGetError returns " << err;
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            qDebug() << "This system can not use QOpenGLDebugLogger, so we revert to glGetError()";

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
            {
                //Turn off Notifications
                mOpenGLDebugLogger->disableMessages(QOpenGLDebugMessage::AnySource, QOpenGLDebugMessage::AnyType, QOpenGLDebugMessage::NotificationSeverity);
                qDebug() << "Started OpenGL debug logger!";
            }
        }
    }
}

void RenderWindow::handleInput()
{
    //Camera
    mCurrentCamera->setSpeed(0.f);  //cancel last frame movement

    if(mInput.F && mPickedObject)
    {
        mCurrentCamera->mPosition = mPickedObject->mTransform.position;
        mCurrentCamera->mPosition.z += (mPickedObject->mCollider.mMaxRadius.z+2)*3; //dynamic distance to object
        mCurrentCamera->mPosition.y += mPickedObject->mCollider.mMaxRadius.y/2;     //dynamic height placement
        mCurrentCamera->mPitch = 0.f;
        mCurrentCamera->mYaw = 0.f;
        mCurrentCamera->update();
    }
    if(mInput.RMB && !ResourceManager::getInstance().mCoreEngine->bTogglePlay)
    {
        //Dirty speedup when Shift is pressed:
        //Seems to kick in on autorepeate of the keyboard
        float tempSpeed = mCameraSpeed;
        Qt::KeyboardModifiers temp = QGuiApplication::keyboardModifiers();
        if(temp == Qt::ShiftModifier)
            tempSpeed *= 2.5f;

        if(mInput.W)
            mCurrentCamera->setSpeed(-tempSpeed);
        if(mInput.S)
            mCurrentCamera->setSpeed(tempSpeed);
        if(mInput.D)
            mCurrentCamera->moveRight(tempSpeed);
        if(mInput.A)
            mCurrentCamera->moveRight(-tempSpeed);
        if(mInput.Q)
            mCurrentCamera->updateHeigth(-tempSpeed);
        if(mInput.E)
            mCurrentCamera->updateHeigth(tempSpeed);
    }
    else    //very manual way to make player move
    {
        float playerSpeed;
        if (mScene1->mEntities[2].bPicked) playerSpeed = 0.02f * mScene1->mEntities[mPlayerID].mScript.pSpeed * 0.5;
        else playerSpeed = 0.02f * mScene1->mEntities[mPlayerID].mScript.pSpeed;

        //Dirty run when Shift is pressed:
        Qt::KeyboardModifiers temp = QGuiApplication::keyboardModifiers();
        if(temp == Qt::ShiftModifier)
            playerSpeed = 0.1f * mScene1->mEntities[mPlayerID].mScript.pSpeed;

        if (ResourceManager::getInstance().mCoreEngine->bTogglePlay && !mScene1->mEntities[mPlayerID].bHidden)
        {
            if(mInput.W)
            {
                mScene1->testWallCollision(mPlayerID, 'f');
                mScene1->mEntities[mPlayerID].mVelocity = {-playerSpeed, 0.f, 0.f};
                mScene1->mEntities[mPlayerID].move();
                mScene1->mEntities[mPlayerID].mTransform.rotation.y = 180;
            }
            if(mInput.S)
            {
                mScene1->testWallCollision(mPlayerID, 'b');
                mScene1->mEntities[mPlayerID].mVelocity = {playerSpeed, 0.f, 0.f};
                mScene1->mEntities[mPlayerID].move();
                mScene1->mEntities[mPlayerID].mTransform.rotation.y = 0;
            }
            if(mInput.D)
            {
                mScene1->testWallCollision(mPlayerID, 'r');
                mScene1->mEntities[mPlayerID].mVelocity = {0.f, 0.f, -playerSpeed};
                mScene1->mEntities[mPlayerID].move();
                mScene1->mEntities[mPlayerID].mTransform.rotation.y = -90;
            }
            if(mInput.A)
            {
                mScene1->testWallCollision(mPlayerID, 'l');
                mScene1->mEntities[mPlayerID].mVelocity = {0.f, 0.f, playerSpeed};
                mScene1->mEntities[mPlayerID].move();
                mScene1->mEntities[mPlayerID].mTransform.rotation.y = 90;
            }
        }
    }
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
        return;

    else
    {
        if (event->key() == Qt::Key_Escape) //Shuts down whole program
        {
            mMainWindow->close();
        }
        if(event->key() == Qt::Key_W)
        {
            mInput.W = true;
        }
        if(event->key() == Qt::Key_S)
        {
            mInput.S = true;
        }
        if(event->key() == Qt::Key_D)
        {
            mInput.D = true;
        }
        if(event->key() == Qt::Key_A)
        {
            mInput.A = true;
        }
        if(event->key() == Qt::Key_Q)
        {
            mInput.Q = true;
        }
        if(event->key() == Qt::Key_E)
        {
            mInput.E = true;
            if (ResourceManager::getInstance().mCoreEngine->bTogglePlay) mScene1->pickObject();
        }
        if(event->key() == Qt::Key_F)
        {
            mInput.F = true;
        }
        if(event->key() == Qt::Key_Z)
        {
            mScene1->mEntities[mLightID].mLight.mAmbientStrength -= 0.1f;
            mScene1->mEntities[mLightID].mLight.mLightStrength -= 0.1f;
        }
        if(event->key() == Qt::Key_C)
        {
            mScene1->mEntities[mLightID].mLight.mAmbientStrength += 0.1f;
            mScene1->mEntities[mLightID].mLight.mLightStrength += 0.1f;
        }

        if(event->key() == Qt::Key_X)
        {
            if (ResourceManager::getInstance().mCoreEngine->bTogglePlay)
            {
                if (!mScene1->mEntities[60].bHidden) mScene1->mEntities[60].bHidden = true;
                else mScene1->mEntities[60].bHidden = false;
            }
        }
        if(event->key() == Qt::Key_Up)
        {
            mInput.UP = true;
        }
        if(event->key() == Qt::Key_Down)
        {
            mInput.DOWN = true;
        }
        if(event->key() == Qt::Key_Left)
        {
            mInput.LEFT = true;
        }
        if(event->key() == Qt::Key_Right)
        {
            mInput.RIGHT = true;
        }
        if(event->key() == Qt::Key_U)
        {        }
        if(event->key() == Qt::Key_O)
        {        }
        if(event->key() == Qt::Key_K)
        {        }
        if(event->key() == Qt::Key_R)
        {
            mScene1->reset();
        }
        if(event->key() == Qt::Key_V)
        {
            ResourceManager::getInstance().mCoreEngine->mScriptSystem->readData(mScene1->mEntities[mPlayerID].mScript.mName);
        }
        if(event->key() == Qt::Key_I)
        {
            mScene1->mEntities[mCamID].mTransform.rotation.z -= 1;
        }
        if(event->key() == Qt::Key_K)
        {
            mScene1->mEntities[mCamID].mTransform.rotation.z += 1;
        }
        if(event->key() == Qt::Key_L)
        {
            mScene1->mEntities[mCamID].mTransform.rotation.y -= 1;
        }
        if(event->key() == Qt::Key_J)
        {
            mScene1->mEntities[mCamID].mTransform.rotation.y += 1;
        }

        if(event->key() == Qt::Key_Tab)
        {
            if (!bPlayerCam) bPlayerCam = true;
            else bPlayerCam = false;
        }
    }
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
        return;

    else
    {
        if(event->key() == Qt::Key_W)
        {
            mInput.W = false;
        }
        if(event->key() == Qt::Key_S)
        {
            mInput.S = false;
        }
        if(event->key() == Qt::Key_D)
        {
            mInput.D = false;
        }
        if(event->key() == Qt::Key_A)
        {
            mInput.A = false;
        }
        if(event->key() == Qt::Key_Q)
        {
            mInput.Q = false;
        }
        if(event->key() == Qt::Key_E)
        {
            mInput.E = false;
        }
        if(event->key() == Qt::Key_F)
        {
            mInput.F = false;
        }
        if(event->key() == Qt::Key_Z)
        {        }
        if(event->key() == Qt::Key_X)
        {        }
        if(event->key() == Qt::Key_Up)
        {
            mInput.UP = false;
        }
        if(event->key() == Qt::Key_Down)
        {
            mInput.DOWN = false;
        }
        if(event->key() == Qt::Key_Left)
        {
            mInput.LEFT = false;
        }
        if(event->key() == Qt::Key_Right)
        {
            mInput.RIGHT = false;
        }
        if(event->key() == Qt::Key_U)
        {        }
        if(event->key() == Qt::Key_O)
        {        }
    }
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = true;
    if (event->button() == Qt::LeftButton)
    {
        mInput.LMB = true;
        if (!mMainWindow->isRunning) mousePicking();
    }
    if (event->button() == Qt::MiddleButton)
        mInput.MMB = true;
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = false;
    if (event->button() == Qt::LeftButton)
        mInput.LMB = false;
    if (event->button() == Qt::MiddleButton)
        mInput.MMB = false;
}

void RenderWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    //if RMB, change the speed of the camera
    if (mInput.RMB)
    {
        if (numDegrees.y() < 1)
            setCameraSpeed(0.001f);
        if (numDegrees.y() > 1)
            setCameraSpeed(-0.001f);
    }
    event->accept();
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mInput.RMB)
    {
        //Using mMouseXYlast as deltaXY so we don't need extra variables
        mMouseXlast = event->pos().x() - mMouseXlast;
        mMouseYlast = event->pos().y() - mMouseYlast;

        if (mMouseXlast != 0)
            mCurrentCamera->yaw(mCameraRotateSpeed * mMouseXlast);
        if (mMouseYlast != 0)
            mCurrentCamera->pitch(mCameraRotateSpeed * mMouseYlast);
    }
    mMouseXlast = event->pos().x();
    mMouseYlast = event->pos().y();
}

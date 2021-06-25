#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class RenderWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QWidget *mRenderWindowContainer{nullptr};

    void updateUI(const std::vector<class Entity> &GameObjectData);

    void selectObjectByName(QString nameIn);

    bool isRunning{false};
    bool renderMousePick{false};

    class WidgetTransform* mTransformWidget{nullptr};
    class WidgetMesh* mMeshWidget{nullptr};
    class WidgetMaterial* mMaterialWidget{nullptr};
    class WidgetSound* mSoundWidget{nullptr};

    class CoreEngine* mCoreEngine;

private slots:
    void on_actionGrid_toggled(bool arg);

    void on_actionAxis_toggled(bool arg);

    void on_actionCorner_Gizmo_toggled(bool arg);

    void on_actionWireframe_toggled(bool arg1);

    void on_actionDraw_Normals_toggled(bool arg);

    void on_actionBasic_Controls_triggered();

    void on_treeWidgetSceneOutliner_itemClicked(class QTreeWidgetItem *item, int);

    void on_actionPlay_toggled(bool arg1);

    void on_actionRender_MousePick_toggled(bool arg1);

    void on_lineEditGOName_returnPressed();

    void on_positionStep_valueChanged(double arg1);

    void on_rotationStep_valueChanged(double arg1);

    void on_scaleStep_valueChanged(double arg1);

    void on_actionTriangle_triggered();

    void on_actionQuad_triggered();

    void on_actionCube_triggered();

    void on_actionSphere_triggered();

    void on_actionFrustum_Culling_toggled(bool arg1);

    void on_actionLOD_toggled(bool arg1);

    void on_actionSound_triggered(bool checked);

private:
    void init();
    Ui::MainWindow *ui;

    RenderWindow *mRenderWindow{nullptr};
    QTreeWidgetItem *mCurrentEditItem{nullptr};
    int mCurrentEditItemIndex{-1};  //index into Scene::mVisualObjects

    void clearLayout(class QLayout *layout);

    //default steps when using transform widget
    float mPositionStep{1.f};
    float mRotationStep{1.f};
    float mScaleStep{0.1f};
};

#endif // MAINWINDOW_H

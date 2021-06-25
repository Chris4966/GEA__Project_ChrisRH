#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <string>

#include "input.h"
#include "gslpch.h"

class QOpenGLContext;
class MainWindow;
class VisualObject;
class Scene;
class Entity;

// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using glad and glw from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override = default;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;

    void toggleWireframe(bool buttonState);
    void spawnEntity(std::string type);
    void reset();

    bool mRenderEditorGrid{true};
    bool mRenderEditorAxis{true};
    bool mRenderCornerGizmo{true};

    void checkForGLerrors();

    //VisualObject *mPlayer{nullptr};  //the interactive object - input works on this
    MAX_ENTITIES_TYPE mPlayerID;
    MAX_ENTITIES_TYPE mCamID;
    MAX_ENTITIES_TYPE mLightID;
    MAX_ENTITIES_TYPE mSurfaceID;
    Scene *mScene1{nullptr};
    bool mDrawNormals{false};

    void setPickedObject(int pickedID);
    void cancelPickedObject();
    void setFocus();

private slots:
    void render();

public:
    void init();

    void calculateFramerate();
    void startOpenGLDebugger();
    void setCameraSpeed(float value);
    void handleInput();

    gsl::mat4* mIdentityMatrix{nullptr};

    int mTextureUnits;      //Supported Texture Units (slots) pr shader.

    class Camera *mCurrentCamera{nullptr};
    float mAspectratio{1.f};

    VisualObject * mEditorAssets[gsl::EditorAssetNumber]{nullptr};
    Input mInput;
    float mCameraSpeed{0.05f};
    float mCameraRotateSpeed{0.1f};
    int mMouseXlast{0};
    int mMouseYlast{0};

    bool bPlayerCam{false}; //camera follows player or not
    bool bToggleFrustum{false}; //toggle frustum culling
    bool bToggleLOD{false}; //toggle LOD
    bool bToggleSound{false}; //toggle sound fx and music

    Entity *mPickedObject{nullptr};
    VisualObject *mPickedObjectMarker{nullptr}; //The box around the picked object

    qreal mRetinaScale{1};  //used on "double pixle/retina" displays

    QOpenGLContext *mContext{nullptr};
    bool mInitialized;

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    void shaderAndMaterialSetup();
    void renderEditorAssets();

    void mousePicking();

public:
    //The QWindow that we inherit from has these functions to capture mouse and keyboard.
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // RENDERWINDOW_H

#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "gslpch.h"
#include "shader.h"
#include "ecs/components.h"

class RenderWindow;

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
    VisualObject() = default;
    virtual ~VisualObject();
    virtual void init();
    virtual void draw()=0;
    void drawOutline();

    ///Use to draw the normals of this mesh
    void drawNormals(GLint matrixUniform);

    ///Draw object with separate color for mousepicking
    void drawMousePicker(GLint matrixUniform, GLint colorUniform, gsl::vec3 color);

    void updateMatrix();

    ///Should this objects normals be drawn from drawNormals()-function - set this from editor (eventually)
    bool mDrawNormals{true};
    bool mDrawMesh{true};   //should mesh be drawn

    gsl::mat4 mMatrix;

    //components:
    ecs::Transform mTransform{};
    ecs::Collider mCollider;

    std::string mName{"noname"};

    ecs::Material *mMaterial{nullptr};

    RenderWindow *mRenderWindow{nullptr}; //Just to be able to call checkForGLerrors()

public:
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;
    unsigned short mIndiceCount{0};

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)
};
#endif // VISUALOBJECT_H


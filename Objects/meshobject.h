#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include <QOpenGLFunctions_4_1_Core>

#include "vertex.h"
#include "gltypes.h"
#include "gslpch.h"
#include "ecs/components.h"

class MeshObject : public QOpenGLFunctions_4_1_Core
{
public:
    MeshObject(std::string filename);

    void init();

    void readFile();
    void makeQuad();
    void makeTriangle();
    void makeCube();
    void makeSkybox();

    /**
      Gets called when mesh is made, pr vertex made/read.
      This is so we don´t need to go thru all vertices once more at the end.
       @param vertexIn  The vertex that is to be checked
     */
    void makeOBB(gsl::vec3 &vertexIn);

    void makeFaces();

    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    std::string mMeshName;

    ecs::Mesh mMesh;
    GLuint mVBO{0};
    GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)

    gsl::Colliders mType{gsl::NOCOLLIDER};
    gsl::vec3 mMinCenter{-.5f, -.5f, -.5f};     //xyzMin if box, center if sphere
    gsl::vec3 mMaxRadius{.5f, .5f, .5f};        //xyzMax if box, radius if sphere
};

#endif // MESHOBJECT_H

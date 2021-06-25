#include "skybox.h"
#include "renderwindow.h"
#include "gslpch.h"
#include "resourcemanager.h"

SkyBox::SkyBox()
{
    mVertices.insert( mVertices.end(),
    {//Vertex data for front
     Vertex{gsl::vec3(-1.f, -1.f, 1.f),    gsl::vec3(0.f, 0.f, 1.0f),  gsl::vec2(0.25f, 0.333f)},  //v0
     Vertex{gsl::vec3( 1.f, -1.f, 1.f),    gsl::vec3(0.f, 0.f, 1.0f),  gsl::vec2(0.5f,  0.333f)},  //v1
     Vertex{gsl::vec3(-1.f,  1.f, 1.f),    gsl::vec3(0.f, 0.f, 1.0f),  gsl::vec2(0.25f, 0.666f)},    //v2
     Vertex{gsl::vec3( 1.f,  1.f, 1.f),    gsl::vec3(0.f, 0.f, 1.0f),  gsl::vec2(0.5f,  0.666f)},    //v3

     //Vertex data for right
     Vertex{gsl::vec3(1.f, -1.f,  1.f),    gsl::vec3(1.f, 0.f, 0.f),   gsl::vec2(0.5f,  0.333f)},  //v4
     Vertex{gsl::vec3(1.f, -1.f, -1.f),    gsl::vec3(1.f, 0.f, 0.f),   gsl::vec2(0.75f, 0.333f)},  //v5
     Vertex{gsl::vec3(1.f,  1.f,  1.f),    gsl::vec3(1.f, 0.f, 0.f),   gsl::vec2(0.5f,  0.666f)},    //v6
     Vertex{gsl::vec3(1.f,  1.f, -1.f),    gsl::vec3(1.f, 0.f, 0.f),   gsl::vec2(0.75f, 0.666f)},    //v7

     //Vertex data for back
     Vertex{gsl::vec3( 1.f, -1.f, -1.f),   gsl::vec3(0.f, 0.f, -1.f),  gsl::vec2(0.75f, 0.333f)},  //v8
     Vertex{gsl::vec3(-1.f, -1.f, -1.f),   gsl::vec3(0.f, 0.f, -1.f),  gsl::vec2(1.f,   0.333f)},  //v9
     Vertex{gsl::vec3( 1.f,  1.f, -1.f),   gsl::vec3(0.f, 0.f, -1.f),  gsl::vec2(0.75f, 0.666f)},    //v10
     Vertex{gsl::vec3(-1.f,  1.f, -1.f),   gsl::vec3(0.f, 0.f, -1.f),  gsl::vec2(1.f,   0.666f)},    //v11

     //Vertex data for left
     Vertex{gsl::vec3(-1.f, -1.f, -1.f),   gsl::vec3(-1.f, 0.f, 0.f),  gsl::vec2(0.f, 0.333f)},    //v12
     Vertex{gsl::vec3(-1.f, -1.f,  1.f),   gsl::vec3(-1.f, 0.f, 0.f),  gsl::vec2(0.25f, 0.333f)},  //v13
     Vertex{gsl::vec3(-1.f,  1.f, -1.f),   gsl::vec3(-1.f, 0.f, 0.f),  gsl::vec2(0.f, 0.666f)},      //v14
     Vertex{gsl::vec3(-1.f,  1.f,  1.f),   gsl::vec3(-1.f, 0.f, 0.f),  gsl::vec2(0.25f,   0.666f)},  //v15

     //Vertex data for bottom
     Vertex{gsl::vec3(-1.f, -1.f, -1.f),   gsl::vec3(0.f, -1.f, 0.f),  gsl::vec2(0.25f, 0.f)},      //v16
     Vertex{gsl::vec3( 1.f, -1.f, -1.f),   gsl::vec3(0.f, -1.f, 0.f),  gsl::vec2(0.5f,  0.f)},      //v17
     Vertex{gsl::vec3(-1.f, -1.f,  1.f),   gsl::vec3(0.f, -1.f, 0.f),  gsl::vec2(0.25f, 0.333f)},   //v18
     Vertex{gsl::vec3( 1.f, -1.f,  1.f),   gsl::vec3(0.f, -1.f, 0.f),  gsl::vec2(0.5f,  0.333f)},   //v19

     //Vertex data for top
     Vertex{gsl::vec3(-1.f, 1.f,  1.f),    gsl::vec3(0.f, 1.f, 0.f),   gsl::vec2(0.25f, 0.666f)},    //v20
     Vertex{gsl::vec3( 1.f, 1.f,  1.f),    gsl::vec3(0.f, 1.f, 0.f),   gsl::vec2(0.5f,  0.666f)},    //v21
     Vertex{gsl::vec3(-1.f, 1.f, -1.f),    gsl::vec3(0.f, 1.f, 0.f),   gsl::vec2(0.25f, 0.999f)},      //v22
     Vertex{gsl::vec3( 1.f, 1.f, -1.f),    gsl::vec3(0.f, 1.f, 0.f),   gsl::vec2(0.5f,  0.999f)}       //v23
                      });

    mIndices.insert( mIndices.end(),
    { 0,  2,  1,  1,  2,  3,      //Face 0 - triangle strip (v0,  v1,  v2,  v3)
      4,  6,  5,  5,  6,  7,      //Face 1 - triangle strip (v4,  v5,  v6,  v7)
      8,  10,  9, 9, 10, 11,      //Face 2 - triangle strip (v8,  v9, v10,  v11)
      12, 14, 13, 13, 14, 15,     //Face 3 - triangle strip (v12, v13, v14, v15)
      16, 18, 17, 17, 18, 19,     //Face 4 - triangle strip (v16, v17, v18, v19)
      20, 22, 21, 21, 22, 23      //Face 5 - triangle strip (v20, v21, v22, v23)
                     });
    mMatrix.setToIdentity();
    mMatrix.scale(250.f);
    init();
}

void SkyBox::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0  );          // array buffer offset

    glEnableVertexAttribArray(0);

    //Does not need color and uvs
    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

//    Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void SkyBox::draw()
{
    glBindVertexArray( mVAO );
    ResourceManager::getInstance().mShaderProgram[mMaterial->mShader]->use(mMatrix, mMaterial);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
//    glDrawArrays(GL_TRIANGLES, 0, 108);
    mRenderWindow->checkForGLerrors();
    glBindVertexArray(0);
}

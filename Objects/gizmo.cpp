#include "gizmo.h"
#include "vertex.h"
#include "shader.h"
#include "resourcemanager.h"

Gizmo::Gizmo()
{
    mVertices.emplace_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});  // X-axis
    mVertices.emplace_back(Vertex{.1f, 0.f, 0.f, 1.f, 0.f, 0.f});
    mVertices.emplace_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});  // Y-axis
    mVertices.emplace_back(Vertex{0.f, .1f, 0.f, 0.f, 1.f, 0.f});
    mVertices.emplace_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});  // Z-axis
    mVertices.emplace_back(Vertex{0.f, 0.f, .1f, 0.f, 0.f, 1.f});
    mMatrix.setToIdentity();
}

Gizmo::~Gizmo()
{
}

void Gizmo::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Gizmo::draw()
{
    glBindVertexArray( mVAO );

    ResourceManager::getInstance().mShaderProgram[mMaterial->mShader]->use(mMatrix, mMaterial);
    glDrawArrays(GL_LINES, 0, mVertices.size());
}

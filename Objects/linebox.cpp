#include "linebox.h"
#include "vertex.h"
#include "resourcemanager.h"
#include "ecs/components.h"

LineBox::LineBox(const gsl::vec3 &lowerLeftFront, const gsl::vec3 &upperRightBack)
{
    float xMinus{lowerLeftFront.x};
    float xPlus{upperRightBack.x};
    float yMinus{lowerLeftFront.y};
    float yPlus{upperRightBack.y};
    float zMinus{upperRightBack.z};
    float zPlus{lowerLeftFront.z};

    mVertices.insert( mVertices.end(),
      {//Vertex data for front points       color                       uv
       Vertex{xMinus, yMinus, zPlus,       1, 0.301, 0.933,          0.f, 0.f},     // 0
       Vertex{xPlus,  yMinus, zPlus,       1, 0.301, 0.933,          0.f, 0.f},
       Vertex{xPlus,  yPlus,  zPlus,       1, 0.301, 0.933,          0.f, 0.f},
       Vertex{xMinus, yPlus,  zPlus,       1, 0.301, 0.933,          0.f, 0.f},
       //Vertex data for back
       Vertex{xMinus, yMinus, zMinus,      1, 0.301, 0.933,          0.f, 0.f},    // 4
       Vertex{xPlus,  yMinus, zMinus,      1, 0.301, 0.933,          0.f, 0.f},
       Vertex{xPlus,  yPlus,  zMinus,      1, 0.301, 0.933,          0.f, 0.f},
       Vertex{xMinus, yPlus,  zMinus,      1, 0.301, 0.933,          0.f, 0.f},
                      });

    //One line at a time
    mIndices.insert( mIndices.end(),
    { 0, 1, 1, 2, 2, 3, 3, 0,       //front rectangle
      4, 5, 5, 6, 6, 7, 7, 4,       //back rectangle
      0, 4, 3, 7,                   //leftside lines
      1, 5, 2, 6                    //rightside lines
                     });

    mMatrix.setToIdentity();
    init();
}

void LineBox::init()
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

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    mIndiceCount = mIndices.size();

    glBindVertexArray(0);
}

void LineBox::draw()
{
    glBindVertexArray( mVAO );
    ResourceManager::getInstance().mShaderProgram[mMaterial->mShader]->use(mMatrix, mMaterial);
    glDrawElements(GL_LINES, mIndiceCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

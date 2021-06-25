#include "editorgrid.h"

EditorGrid::EditorGrid() : EditorGrid(10, 1)
{
}

EditorGrid::EditorGrid(int size, int scale) : mSize{size}, mScale{scale}
{
    float startPoint = -mSize*mScale;

    float lineStart = startPoint;
    float lineEnd = -startPoint;

    float mainColor{0.3f};
    float altColor{0.5f};
    float color{0.f};

    bool altColorFlip{true};

    float altMainAxisColor{.45f};
    float yHeight{-0.001f};     //to avoid z-fighting with axis

    //lines parallell to Z
    for(float i{startPoint}; i<= (startPoint*-1); i += scale)
    {
        //special case for  0 line:
        if (i >=-0.1f && i<= 0.1f)
        {
            mVertices.emplace_back(Vertex{i, yHeight, lineStart,       altMainAxisColor, altMainAxisColor, 1.f});
            mVertices.emplace_back(Vertex{i, yHeight, lineEnd,         altMainAxisColor, altMainAxisColor, 1.f});
            altColorFlip = !altColorFlip;
        }
        else
        {
            altColorFlip ? color = mainColor : color = altColor;
            mVertices.emplace_back(Vertex{i, yHeight, lineStart, color, color, color});
            mVertices.emplace_back(Vertex{i, yHeight, lineEnd, color, color, color});
            altColorFlip = !altColorFlip;
        }
    }

    altColorFlip = true;    //reset for X-lines
    //lines parallell to X
    for(float i{startPoint}; i<= (startPoint*-1); i += scale)
    {
        //special case for  0 line:
        if (i >=-0.1f && i<= 0.1f)
        {
            mVertices.emplace_back(Vertex{lineStart, yHeight, i,       1.f, altMainAxisColor, altMainAxisColor});
            mVertices.emplace_back(Vertex{lineEnd, yHeight, i,         1.f, altMainAxisColor, altMainAxisColor});
            altColorFlip = !altColorFlip;
        }
        else
        {
            altColorFlip ? color = mainColor : color = altColor;
            mVertices.emplace_back(Vertex{lineStart, yHeight, i, color, color, color});
            mVertices.emplace_back(Vertex{lineEnd, yHeight, i, color, color, color});
            altColorFlip = !altColorFlip;
        }
    }

//    qDebug() << "mVertices for grid:" << mVertices.size();

    mMatrix.setToIdentity();
}

EditorGrid::~EditorGrid()
{
}

void EditorGrid::init()
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
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void EditorGrid::draw()
{
    glBindVertexArray( mVAO );
    glDrawArrays(GL_LINES, 0, mVertices.size());
}

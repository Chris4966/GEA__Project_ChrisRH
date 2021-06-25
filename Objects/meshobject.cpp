#include "meshobject.h"

#include <sstream>
#include "resourcemanager.h"

MeshObject::MeshObject(std::string filename) : mMeshName{filename}
{
    if (filename.find(".obj") != std::string::npos)
        readFile();
    if(filename == "plane")
        makeQuad();
    if(filename == "triangle")
        makeTriangle();
    if(filename == "cube")
        makeCube();
    if(filename == "skybox")
        makeSkybox();

    init();
}

void MeshObject::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mMesh.mVAO );
    glBindVertexArray( mMesh.mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    if(mIndices.size() > 0) {
        glGenBuffers(1, &mEAB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);

    mMesh.mIndexCount = mIndices.size();
    mMesh.mVertexCount = mVertices.size();
    //makeFaces();
}

void MeshObject::readFile()
{
    //Open File
    //    std::string filename = Orf::assetFilePath.toStdString() + fileName + ".obj";
    std::ifstream fileIn;
    fileIn.open (mMeshName, std::ifstream::in);
    if(!fileIn)
        qDebug() << "Could not open file for reading: " << QString::fromStdString(mMeshName);

    //One line at a time-variable
    std::string oneLine;
    //One word at a time-variable
    std::string oneWord;

    std::vector<gsl::vec3> tempVertecies;
    std::vector<gsl::vec3> tempNormals;
    std::vector<gsl::vec2> tempUVs;

    // Varible for constructing the indices vector
    unsigned int temp_index = 0;

    //Reading one line at a time from file to oneLine
    while(std::getline(fileIn, oneLine))
    {
        //Doing a trick to get one word at a time
        std::stringstream sStream;
        //Pushing line into stream
        sStream << oneLine;
        //Streaming one word out of line
        oneWord = ""; //resetting the value or else the last value might survive!
        sStream >> oneWord;

        if (oneWord == "#")
        {
            //Ignore this line
            //            qDebug() << "Line is comment "  << QString::fromStdString(oneWord);
            continue;
        }
        if (oneWord.empty())
        {
            //Ignore this line
            //            qDebug() << "Line is blank ";
            continue;
        }
        if (oneWord == "v")
        {
            //            qDebug() << "Line is vertex "  << QString::fromStdString(oneWord) << " ";
            gsl::vec3 tempVertex;
            sStream >> oneWord;
            tempVertex.x = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.y = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.z = std::stof(oneWord);

            //Vertex made - pushing it into vertex-vector
            tempVertecies.push_back(tempVertex);
            makeOBB(tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
            //            qDebug() << "Line is UV-coordinate "  << QString::fromStdString(oneWord) << " ";
            gsl::vec2 tempUV;
            sStream >> oneWord;
            tempUV.x = std::stof(oneWord);
            sStream >> oneWord;
            tempUV.y = std::stof(oneWord);

            //UV made - pushing it into UV-vector
            tempUVs.push_back(tempUV);

            continue;
        }
        if (oneWord == "vn")
        {
            //            qDebug() << "Line is normal "  << QString::fromStdString(oneWord) << " ";
            gsl::vec3 tempNormal;
            sStream >> oneWord;
            tempNormal.x = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.y = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.z = std::stof(oneWord);

            //Vertex made - pushing it into vertex-vector
            tempNormals.push_back(tempNormal);
            continue;
        }
        if (oneWord == "f")
        {
            //            qDebug() << "Line is a face "  << QString::fromStdString(oneWord) << " ";
            //int slash; //used to get the / from the v/t/n - format
            int index, normal, uv;
            for(int i = 0; i < 3; i++)
            {
                sStream >> oneWord;     //one word read
                std::stringstream tempWord(oneWord);    //to use getline on this one word
                std::string segment;    //the numbers in the f-line
                std::vector<std::string> segmentArray;  //temp array of the numbers
                while(std::getline(tempWord, segment, '/')) //splitting word in segments
                {
                    segmentArray.push_back(segment);
                }
                index = std::stoi(segmentArray[0]);     //first is vertex
                if (!segmentArray[1].empty())              //second is uv
                    uv = std::stoi(segmentArray[1]);
                else
                {
                    //qDebug() << "No uvs in mesh";       //uv not present
                    uv = 0;                             //this will become -1 in a couple of lines
                }
                normal = std::stoi(segmentArray[2]);    //third is normal

                //Fixing the indexes
                //because obj f-lines starts with 1, not 0
                --index;
                --uv;
                --normal;

                if (uv > -1)    //uv present!
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], tempUVs[uv]);
                    mVertices.push_back(tempVert);
                }
                else            //no uv in mesh data, use 0, 0 as uv
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::vec2(0.0f, 0.0f));
                    mVertices.push_back(tempVert);
                }
                mIndices.push_back(temp_index++);
            }
            continue;
        }
    }

    //beeing a nice boy and closing the file after use
    fileIn.close();
    qDebug() << "Obj-file " << QString::fromStdString(mMeshName) << "successfully read";
}

void MeshObject::makeQuad()
{
    //                          // Positions            // normals        //UV
    mVertices.emplace_back(Vertex(-1.f, 0.f, 1.f,       0, 1, 0,          0.f, 0.f));
    mVertices.emplace_back(Vertex(1.f, 0.f, 1.f,        0, 1, 0,          1.f, 0.f));
    mVertices.emplace_back(Vertex(1.f, 0.f, -1.f,       0, 1, 0,          1.f, 1.f));

    mVertices.emplace_back(Vertex(-1.f, 0.f, 1.f,       0, 1, 0,          0.f, 0.f));
    mVertices.emplace_back(Vertex(1.f, 0.f, -1.f,       0, 1, 0,          1.f, 1.f));
    mVertices.emplace_back(Vertex(-1.f, 0.f, -1.f,      0, 1, 0,          0.f, 1.f));
}

void MeshObject::makeTriangle()
{
    mVertices.emplace_back(Vertex{-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f}); // Bottom Left
    mVertices.emplace_back(Vertex{0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.f}); // Bottom Right
    mVertices.emplace_back(Vertex{0.0f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top
}

void MeshObject::makeCube()
{
    mVertices.emplace_back(Vertex{-1.0f, -1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{-1.0f, -1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{-1.0f,  1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{1.0f,   1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});

    mVertices.emplace_back(Vertex{-1.0f, -1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{-1.0f,  1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{1.0f,  -1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{-1.0f, -1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});

    mVertices.emplace_back(Vertex{1.0f,  -1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{1.0f,   1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{1.0f,  -1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{-1.0f, -1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});

    mVertices.emplace_back(Vertex{-1.0f, -1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{-1.0f,  1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{-1.0f,  1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{1.0f,  -1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});

    mVertices.emplace_back(Vertex{-1.0f, -1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{-1.0f, -1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{-1.0f,  1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{-1.0f, -1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});

    mVertices.emplace_back(Vertex{1.0f,  -1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{1.0f,  -1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{1.0f,   1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});

    mVertices.emplace_back(Vertex{1.0f,  -1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{1.0f,  -1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});

    mVertices.emplace_back(Vertex{1.0f,   1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{-1.0f,  1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{-1.0f,  1.0f,-1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});

    mVertices.emplace_back(Vertex{-1.0f,  1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{-1.0f,  1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
    mVertices.emplace_back(Vertex{1.0f,  -1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f});
}

void MeshObject::makeSkybox()
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
}

void MeshObject::makeOBB(gsl::vec3 &vertexIn)
{
    //testing min
    if(vertexIn.x < mMinCenter.x)
        mMinCenter.x = vertexIn.x;
    if(vertexIn.y < mMinCenter.y)
        mMinCenter.y = vertexIn.y;
    if(vertexIn.z < mMinCenter.z)
        mMinCenter.z = vertexIn.z;

    //testing max
    if(vertexIn.x > mMaxRadius.x)
        mMaxRadius.x = vertexIn.x;
    if(vertexIn.y > mMaxRadius.y)
        mMaxRadius.y = vertexIn.y;
    if(vertexIn.z > mMaxRadius.z)
        mMaxRadius.z = vertexIn.z;
}

void MeshObject::makeFaces()
{
    unsigned int i{0};
    Vertex a, b, c;

    while (i < mVertices.size()-1)
    {
        a = mVertices.at(i++);
        b = mVertices.at(i++);
        c = mVertices.at(i++);

        mMesh.mFaces.push_back(ecs::Face(a, b, c));
    }
}

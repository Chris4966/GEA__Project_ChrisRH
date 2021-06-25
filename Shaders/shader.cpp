#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <QDebug>

#include "constants.h"

//#include "GL/glew.h" - using QOpenGLFunctions instead

Shader::Shader(const std::string &shaderName) : mName{shaderName}
{
    initializeOpenGLFunctions();    //must do this to get access to OpenGL functions in QOpenGLFunctions

    qDebug() << "*** Reading" << shaderName.c_str() << "shader:";

    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    // Open files and check for errors

    std::string vertexPath = (gsl::ShaderFilePath + shaderName.c_str() + ".vert");
//    qDebug() << vertexPath << " \n";
    std::string fragmentPath = (gsl::ShaderFilePath + shaderName.c_str() + ".frag");
//    qDebug() << fragmentPath << " \n";
    std::string geometryPath = (gsl::ShaderFilePath + shaderName.c_str() + ".geom");
    bool haveGeoShader{false};

    vShaderFile.open( vertexPath.c_str() );
    if(!vShaderFile)
        qDebug() << "ERROR VERTEX SHADER FILE " << shaderName.c_str() << " NOT SUCCESFULLY READ";
    fShaderFile.open( fragmentPath.c_str() );
    if(!fShaderFile)
        qDebug() << "ERROR FRAGMENT SHADER FILE " << shaderName.c_str() << " NOT SUCCESFULLY READ";

    gShaderFile.open( geometryPath.c_str() );
    if(gShaderFile)
        haveGeoShader = true;
//    else
//        qDebug() << shaderName.c_str() << " has no geometry shader";

    std::stringstream vShaderStream;
    std::stringstream fShaderStream;
    std::stringstream gShaderStream;

    // Read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    if(haveGeoShader)
        gShaderStream << gShaderFile.rdbuf();

    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    if(haveGeoShader)
        gShaderFile.close();

    // Convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
    if(haveGeoShader)
        geometryCode = gShaderStream.str();

    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    const GLchar *gShaderCode;

    if(haveGeoShader)
        gShaderCode = geometryCode.c_str();

    // 2. Compile shaders
    GLuint vertex;
    GLuint fragment;
    GLuint geometry;
    GLint success;
    GLchar infoLog[512];
    // Vertex Shader
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, nullptr );
    glCompileShader( vertex );
    // Print compile errors if any
    glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertex, 512, nullptr, infoLog );
        qDebug() << "ERROR SHADER VERTEX " << vertexPath.c_str() << " COMPILATION_FAILED\n" << infoLog;
    }
    // Fragment Shader
    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, nullptr );
    glCompileShader( fragment );
    // Print compile errors if any
    glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( fragment, 512, nullptr, infoLog );
        qDebug() << "ERROR SHADER FRAGMENT " << fragmentPath.c_str() << " COMPILATION_FAILED\n" << infoLog;
    }

    // Geometry Shader
    if(haveGeoShader)
    {
        geometry = glCreateShader( GL_GEOMETRY_SHADER );
        glShaderSource( geometry, 1, &gShaderCode, nullptr );
        glCompileShader( geometry );
        // Print compile errors if any
        glGetShaderiv( geometry, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( geometry, 512, nullptr, infoLog );
            qDebug() << "ERROR SHADER GEOMETRY " << geometryPath.c_str() << " COMPILATION_FAILED\n" << infoLog;
        }
    }

    // Shader Program
    this->mProgram = glCreateProgram();
    glAttachShader(this->mProgram, vertex);
    glAttachShader(this->mProgram, fragment);
    if(haveGeoShader)
        glAttachShader(this->mProgram, geometry);
    glLinkProgram( this->mProgram );
    // Print linking errors if any
    glGetProgramiv( this->mProgram, GL_LINK_STATUS, &success );
    if (!success)
    {
        glGetProgramInfoLog( this->mProgram, 512, nullptr, infoLog );
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED " << shaderName.c_str() <<  "\n   " << infoLog;
    }
    else
    {
        qDebug() << shaderName.c_str() << "was successfully compiled!" << "id:" << mProgram;
    }
    // Delete the shaders as they're linked into our program now and no longer needed
    glDeleteShader( vertex );
    glDeleteShader( fragment );
}

void Shader::use(gsl::mat4 &modelMatrixIn, ecs::Material *)
{
    glUseProgram( this->mProgram );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, modelMatrixIn.constData());
}

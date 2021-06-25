#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_4_1_Core>
#include <string>
#include "ecs/components.h"
#include "gslpch.h"

//This class is pretty much a copy of the shader class at
//https://github.com/SonarSystems/Modern-OpenGL-Tutorials/blob/master/%5BLIGHTING%5D/%5B8%5D%20Basic%20Lighting/Shader.h
//which is based on stuff from http://learnopengl.com/ and http://open.gl/.

class Shader : protected QOpenGLFunctions_4_1_Core
{
public:
    // Constructor generates the shader on the fly
    Shader(const std::string &shaderName);

    // Use the current shader
    virtual void use(gsl::mat4 &modelMatrixIn, struct ecs::Material *);
    virtual void setupShader(){}

    const std::string mName;

    GLint mMatrixUniform{-1};       //when using the shader in drawcalls to different objects
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};

    class Camera *mCurrentCamera{nullptr};
    class RenderWindow *mRenderWindow{nullptr};     //to be able to call checkForGLerrors()

    GLuint mProgram{999999};
};

#endif

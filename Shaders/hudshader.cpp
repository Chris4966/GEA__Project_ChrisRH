#include "hudshader.h"
#include "camera.h"

HUDShader::HUDShader(const std::string &shaderName) : Shader(shaderName)
{ }

void HUDShader::setupShader()
{
    mMatrixUniform = glGetUniformLocation( mProgram, "mMatrix" );
}

void HUDShader::use(gsl::mat4 &modelMatrixIn, ecs::Material *)
{
    glUseProgram(mProgram);
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, modelMatrixIn.constData());
}

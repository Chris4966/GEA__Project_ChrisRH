#include "singlecolorshader.h"
#include "camera.h"
#include "ecs/components.h"

SingleColorShader::SingleColorShader(const std::string &shaderName)  : Shader(shaderName)
{ }

void SingleColorShader::setupShader()
{
    mMatrixUniform = glGetUniformLocation( mProgram, "mMatrix" );
    vMatrixUniform = glGetUniformLocation( mProgram, "vMatrix" );
    pMatrixUniform = glGetUniformLocation( mProgram, "pMatrix" );

    mObjectColorUniform = glGetUniformLocation( mProgram, "objectColor" );
}

void SingleColorShader::use(gsl::mat4 &modelMatrixIn, ecs::Material * materialIn)
{
    glUseProgram(mProgram);
    glUniformMatrix4fv( vMatrixUniform, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
    glUniformMatrix4fv( pMatrixUniform, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
    glUniform3f(mObjectColorUniform, materialIn->mColor.x, materialIn->mColor.y, materialIn->mColor.z);
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, modelMatrixIn.constData());
}

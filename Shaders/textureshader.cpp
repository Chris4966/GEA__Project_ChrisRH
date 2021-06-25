#include "textureshader.h"

#include "camera.h"
#include "texture.h"

TextureShader::TextureShader(const std::string &shaderName) : Shader(shaderName)
{ }

void TextureShader::setupShader()
{
    mMatrixUniform = glGetUniformLocation( mProgram, "mMatrix" );
    vMatrixUniform = glGetUniformLocation( mProgram, "vMatrix" );
    pMatrixUniform = glGetUniformLocation( mProgram, "pMatrix" );
    mTextureUniform = glGetUniformLocation( mProgram, "textureSampler");
}

void TextureShader::use(gsl::mat4 &modelMatrixIn, ecs::Material *materialIn)
{
    glUseProgram(mProgram);
    glUniformMatrix4fv( vMatrixUniform, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
    glUniformMatrix4fv( pMatrixUniform, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());

    glUniform1i(mTextureUniform, materialIn->mTextureId-1); //should use texture slot!!!
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, modelMatrixIn.constData());
}

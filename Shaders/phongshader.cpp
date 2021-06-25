#include "phongshader.h"

#include "camera.h"
#include "light.h"
#include "texture.h"
#include "ecs/components.h"
#include "resourcemanager.h"

PhongShader::PhongShader(const std::string &shaderName) : Shader(shaderName)
{
}

void PhongShader::use(gsl::mat4 &modelMatrixIn, ecs::Material *materialIn)
{
    glUseProgram(mProgram);
    glUniformMatrix4fv( vMatrixUniform, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
    glUniformMatrix4fv( pMatrixUniform, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());

    glUniform3f(mLightPositionUniform, mTransform->position.x, mTransform->position.y, mTransform->position.z);
    glUniform3f(mCameraPositionUniform, mCurrentCamera->position().x, mCurrentCamera->position().y, mCurrentCamera->position().z);
    glUniform3f(mLightColorUniform, mCurrentLight->mLightColour.x, mCurrentLight->mLightColour.y, mCurrentLight->mLightColour.z);
    glUniform1i(mTextureUniform, materialIn->mTextureId-1);
    glUniform1f(mLightStrenghtUniform, mCurrentLight->mLightStrength);

    glUniform1f(mSpecularStrengthUniform, materialIn->mSpecularStrenght);
    glUniform1i(mSpecularExponentUniform, materialIn->mSpecularExponent);
    glUniform1f(mAmbientLightStrengthUniform, mCurrentLight->mAmbientStrength);
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, modelMatrixIn.constData());
}

void PhongShader::setupShader()
{
    mMatrixUniform = glGetUniformLocation( mProgram, "mMatrix" );
    vMatrixUniform = glGetUniformLocation( mProgram, "vMatrix" );
    pMatrixUniform = glGetUniformLocation( mProgram, "pMatrix" );

    mLightColorUniform =            glGetUniformLocation( mProgram, "lightColor" );
//    mObjectColorUniform =           glGetUniformLocation( mProgram, "objectColor" );
    mAmbientLightStrengthUniform =  glGetUniformLocation( mProgram, "ambientStrengt" );
    mLightPositionUniform =         glGetUniformLocation( mProgram, "lightPosition" );
    mLightStrenghtUniform =         glGetUniformLocation( mProgram, "lightStrengt" );
    mSpecularStrengthUniform =      glGetUniformLocation( mProgram, "specularStrength" );
    mSpecularExponentUniform =      glGetUniformLocation( mProgram, "specularExponent" );
    mCameraPositionUniform =        glGetUniformLocation( mProgram, "cameraPosition" );
    mTextureUniform =               glGetUniformLocation( mProgram, "textureSampler" );
}

#include "mousepickershader.h"
#include "camera.h"

MousePickerShader::MousePickerShader(const std::string &shaderName)  : Shader(shaderName)
{ }

void MousePickerShader::setupShader()
{
    mMatrixUniform = glGetUniformLocation( mProgram, "mMatrix" );
    pMatrixUniform = glGetUniformLocation( mProgram, "vpMatrix" );
    //using this for the VP calculated matrix!
    mPickerColor = glGetUniformLocation(mProgram, "pickingColor");
}

void MousePickerShader::use(gsl::mat4 &modelMatrixIn, ecs::Material *)
{
    glUseProgram(mProgram);
    gsl::mat4 temp = mCurrentCamera->mProjectionMatrix * mCurrentCamera->mViewMatrix;
    glUniformMatrix4fv( pMatrixUniform, 1, GL_TRUE, temp.constData());
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, modelMatrixIn.constData());
}

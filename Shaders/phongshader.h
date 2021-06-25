#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include "shader.h"
#include "gslpch.h"
#include "ecs/components.h"

class PhongShader : public Shader
{
public:
    PhongShader(const std::string &shaderName);

    void setupShader() override;
    void use(gsl::mat4 &modelMatrixIn, ecs::Material *materialIn) override;

    ecs::Light *mCurrentLight{nullptr};
    ecs::Transform *mTransform;

    //light shader variables
    GLint mLightColorUniform{-1};
//    GLint mObjectColorUniform{-1};
    GLint mAmbientLightStrengthUniform{-1};
    GLint mLightPositionUniform{-1};
    GLint mCameraPositionUniform{-1};
    GLint mSpecularStrengthUniform{-1};
    GLint mSpecularExponentUniform{-1};
    GLint mLightStrenghtUniform{-1};
    GLint mTextureUniform{-1};
};

#endif // PHONGSHADER_H

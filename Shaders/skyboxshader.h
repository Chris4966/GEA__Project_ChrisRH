#ifndef SKYBOXSHADER_H
#define SKYBOXSHADER_H

#include "shader.h"
#include "gslpch.h"

class SkyboxShader : public Shader
{
public:
    SkyboxShader(const std::string &shaderName);

    void setupShader() override;
    void use(gsl::mat4 &modelMatrixIn, struct ecs::Material *materialIn) override;

    GLint mTextureUniform{-1};
};

#endif // SKYBOXSHADER_H

#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include "shader.h"
#include "gslpch.h"

class TextureShader : public Shader
{
public:
    TextureShader(const std::string &shaderName);

    void setupShader() override;
    void use(gsl::mat4 &modelMatrixIn, struct ecs::Material *materialIn) override;

    GLint mTextureUniform{-1};
};

#endif // TEXTURESHADER_H

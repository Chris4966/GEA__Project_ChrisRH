#ifndef SINGLECOLORSHADER_H
#define SINGLECOLORSHADER_H

#include "shader.h"
#include "gslpch.h"

class SingleColorShader : public Shader
{
public:
    SingleColorShader(const std::string &shaderName);

    void setupShader() override;
    void use(gsl::mat4 &modelMatrixIn, struct ecs::Material * materialIn = nullptr) override;

    GLint mObjectColorUniform{-1};
};

#endif // SINGLECOLORSHADER_H

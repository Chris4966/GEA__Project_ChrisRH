#ifndef COLORSHADER_H
#define COLORSHADER_H

#include "shader.h"
#include "gslpch.h"

class ColorShader : public Shader
{
public:
    ColorShader(const std::string &shaderName);

    void setupShader() override;
    void use(gsl::mat4 &modelMatrixIn, struct ecs::Material * = nullptr) override;
};

#endif // COLORSHADER_H

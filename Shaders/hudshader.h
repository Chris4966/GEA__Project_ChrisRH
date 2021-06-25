#ifndef HUDSHADER_H
#define HUDSHADER_H

#include "shader.h"
#include "gslpch.h"

class HUDShader : public Shader
{
public:
    HUDShader(const std::string &shaderName);

    void setupShader() override;
    void use(gsl::mat4 &modelMatrixIn, struct ecs::Material * = nullptr) override;
};

#endif // HUDSHADER_H

#ifndef NORMALSSHADER_H
#define NORMALSSHADER_H

#include "shader.h"
#include "gslpch.h"

class NormalsShader : public Shader
{
public:
    NormalsShader(const std::string &shaderName);

    void setupShader() override;
    void use(gsl::mat4 &modelMatrixIn, struct ecs::Material * = nullptr) override;
};

#endif // NORMALSSHADER_H

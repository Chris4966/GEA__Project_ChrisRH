#ifndef MOUSEPICKERSHADER_H
#define MOUSEPICKERSHADER_H

#include "shader.h"
#include "gslpch.h"

class MousePickerShader : public Shader
{
public:
    MousePickerShader(const std::string &shaderName);

    void setupShader() override;
    void use(gsl::mat4 &modelMatrixIn, struct ecs::Material * = nullptr) override;

    GLint mPickerColor{-1};
};

#endif // MOUSEPICKERSHADER_H

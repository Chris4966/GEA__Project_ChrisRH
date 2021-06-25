#ifndef SKYBOX_H
#define SKYBOX_H

#include "visualobject.h"

class SkyBox : public VisualObject
{
public:
    SkyBox();
    ~SkyBox() override = default;

    void init() override;
    void draw() override;

    ///The skybox texture
    GLuint mId{0};
    ///The skybox shader
    GLuint mProgram{999999};
};

#endif // SKYBOX_H

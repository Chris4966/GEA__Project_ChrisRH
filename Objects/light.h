#ifndef LIGHT_H
#define LIGHT_H

#include "visualobject.h"

class Light : public VisualObject
{
public:
    Light();
    virtual void init() override;
    virtual void draw() override;

    GLfloat mAmbientStrenght{0.5f};
    gsl::vec3 mAmbientColor{0.3f, 0.3f, 0.3f};

    GLfloat mLightStrenght{0.7f};
    gsl::vec3 mLightColor{0.9f, 0.9f, 0.9f};

    GLfloat mSpecularStrenght{0.3f};
    GLint mSpecularExponent{4};
};

#endif // LIGHT_H

#ifndef GIZMO_H
#define GIZMO_H

#include <QOpenGLFunctions_4_1_Core>
#include "visualobject.h"

class Gizmo : public VisualObject
{
public:
    Gizmo();
    ~Gizmo() override;

    void init() override;
    void draw() override;
};

#endif // GIZMO_H


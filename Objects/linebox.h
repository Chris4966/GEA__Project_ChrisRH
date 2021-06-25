#ifndef LINEBOX_H
#define LINEBOX_H
#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "gslpch.h"
#include "visualobject.h"

class LineBox : public VisualObject
{
public:
    LineBox(const gsl::vec3 &lowerLeftFront = gsl::vec3{-1, -1, 1},     //Default is a 2x2x2 cube
            const gsl::vec3 &upperRightBack = gsl::vec3{1, 1, -1});
    ~LineBox() override = default;

    void init() override;
    void draw() override;
};

#endif // LINEBOX_H

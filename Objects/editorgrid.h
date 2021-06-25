#ifndef EDITORGRID_H
#define EDITORGRID_H

#include <QOpenGLFunctions_4_1_Core>
#include "visualobject.h"

class EditorGrid : public VisualObject
{
public:
    EditorGrid();
    EditorGrid(int size, int scale);
    ~EditorGrid() override;

    void init() override;
    void draw() override;

private:
    int mSize{0};
    int mScale{0};

};

#endif // EDITORGRID_H

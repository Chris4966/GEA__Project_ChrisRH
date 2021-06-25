#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <QOpenGLFunctions_4_1_Core>
#include "ecs/components.h"
#include "resourcemanager.h"

/**
    @brief System Class for rendering entities.
 */

class RenderSystem : public QOpenGLFunctions_4_1_Core
{
public:
    RenderSystem();
    void Render();
    void init();

    void renderNormals(GLint matrixUniform);
    void renderMousepicker(GLint matrixUniform, GLint colourUniform);

    unsigned int mNumberOfMeshes{0};

    ecs::MaterialID *mMaterialIDs;
};

#endif // RENDERSYSTEM_H

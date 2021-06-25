#ifndef COREENGINE_H
#define COREENGINE_H

#include <vector>
#include "ecs/components.h"
#include "gslpch.h"
#include "scene.h"

/**
    @brief Core Engine Class.
 */

class CoreEngine
{
public:
    CoreEngine();

    //Systems
    class RenderSystem *mRenderSystem{nullptr};
    class ScriptSystem *mScriptSystem{nullptr};

    //Scenes
    std::vector<Scene*> mScenes;
    Scene* mCurrentScene{nullptr};

    bool bTogglePlay{false};
};

#endif // COREENGINE_H

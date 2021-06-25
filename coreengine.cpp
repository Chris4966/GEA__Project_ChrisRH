#include "coreengine.h"

#include "rendersystem.h"
#include "scriptsystem.h"
#include "resourcemanager.h"


CoreEngine::CoreEngine()
{
    mRenderSystem = new RenderSystem();
    mScriptSystem = new ScriptSystem();
    ResourceManager::getInstance().mCoreEngine = this;
}

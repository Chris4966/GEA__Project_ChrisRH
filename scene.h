#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "gltypes.h"
#include "gslpch.h"
#include "ecs/entity.h"
#include "visualobject.h"

/**
    \brief Class for creating a scene with entities.
 */

class Scene
{
public:
    Scene(class RenderWindow *renderWindow, class RenderSystem *renderSystem);

    void init();
    void update();
    void reset();
    void render();
    void renderNormals(GLint matrixUniform);
    void renderMousepicker(GLint matrixUniform, GLint colorUniform);

    /** Lets the player pick up objects.
     */
    void pickObject();

    int mSceneNr{0};
    RenderWindow *mRenderWindow{nullptr};
    ecs::Light *mLight{nullptr};
    std::string mName{"no name"};
    MAX_ENTITIES_TYPE mPlayerID;
    MAX_ENTITIES_TYPE mCameraID;
    MAX_ENTITIES_TYPE mSurfaceID;
    MAX_ENTITIES_TYPE mLightID;
    MAX_ENTITIES_TYPE mKeyID;
    std::vector<MAX_ENTITIES_TYPE> mEnemyIDs;
    std::vector<MAX_ENTITIES_TYPE> mDetectorIDs;


    /** Checks Collision between entity and wall.
       @param entityID - ID of entity
       @param dir - move direction, f = forward, b = back, r = right, l = left
     */
    void testWallCollision(MAX_ENTITIES_TYPE entityID, char dir = 'f');

    class RenderSystem *mRenderSystem{nullptr};

    /** Adds a new entity to the scene.
       @param name - name of the entity
       @param mat - name of the material
       @param pos - position to spawn entity
       @param scale - entity scale
     */
    void addEntity(std::string name, std::string mat, gsl::vec3 pos = {0.f, 0.f, 0.f}, gsl::vec3 scale = {1.f, 1.f, 1.f});

    /** Sets up a light entity in the scene.
       @param name - name of the entity
       @param mat - name of the material
       @param pos - position to spawn entity
       @param scale - entity scale
     */
    void lightSetup(std::string name, std::string mat, gsl::vec3 pos = {0.f, 0.f, 0.f}, gsl::vec3 scale = {1.f, 1.f, 1.f});

    void prepareComponentArrays();

    ecs::MaterialID mMaterialIDs[MAX_COMPONENTS];
    std::vector<Entity> mEntities;

    MAX_ENTITIES_TYPE mEntityCount{MAX_ENTITIES}; //how many entities is created
    MAX_ENTITIES_TYPE getNewEntityID();
};

#endif // SCENE_H

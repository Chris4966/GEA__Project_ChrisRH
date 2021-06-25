#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include "components.h"

/**
    @brief Entity Class.
 */

class Entity
{
public:
    Entity()
    {

    };

    void move()
    {
        mTransform.position += mVelocity;
    }

    MAX_ENTITIES_TYPE mEntityID{MAX_ENTITIES};
    std::string mName{"NoName"};
    gsl::vec3 mVelocity{0.f, 0.f, 0.f};
    gsl::vec3 mInitPos{0.f, 0.f, 0.f};

    std::vector<std::string> mTags; //entity tags
    bool bHidden{false}; //if true - do not render
    bool bMuted{false}; //if true - do not play sound
    bool bCollidable{false}; //if true - has collider
    bool bPicked{false}; //picked up by player
    bool bHasScript{false}; //if the entity has a script
    bool bAIMovement{false}; //if the entity has AI movement
    bool bMoveUp{false}; //move direction for npc
    bool bOpened{false}; //is door opened?
    bool bIgnoreFrustum{false}; //if this entity ignores frustum culling

    float mCurrentHP;

    //Not final version - this is just to make the components work for now!
    MAX_ENTITIES_TYPE mMaterialIDIndex;

    ecs::Mesh mMesh;
    ecs::Transform mTransform;
    ecs::MaterialID mMaterialID;
    ecs::Collider mCollider;
    ecs::Sound mSound;
    ecs::Script mScript;
    ecs::Light mLight;

};

#endif // ENTITY_H

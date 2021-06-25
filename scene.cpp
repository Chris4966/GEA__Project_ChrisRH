#include <QDebug>

#include "scene.h"
#include "light.h"
#include "gslpch.h"
#include "renderwindow.h"
#include "skybox.h"
#include "linebox.h"
#include "resourcemanager.h"
#include "ecs/entity.h"
#include "rendersystem.h"
#include "meshobject.h"
#include "coreengine.h"

Scene::Scene(RenderWindow *renderWindow, RenderSystem *renderSystem) : mRenderWindow{renderWindow}, mRenderSystem{renderSystem}
{
    mRenderSystem->mMaterialIDs = mMaterialIDs;
}

void Scene::init()
{
    ResourceManager &resourceMgr = ResourceManager::getInstance();

    //********************** Making the entities to be drawn **********************

    mLightID = 0;
    lightSetup("bulb.obj", "Outline", {0.f, 6.f, 0.f});
    resourceMgr.setTag(0, "light"); resourceMgr.setTag(0, "invisible");

    //floor
    mSurfaceID = 1;
    addEntity("plane", "Metal", {4.f, 0.01f, 4.f}, {4.5f, 1.f, 4.5f});

    //pickable box
    addEntity("box.obj", "WoodMat", {6.5f, 0.01f, 6.5f}, {0.5f, 0.5f, 0.5f});
    resourceMgr.setTag(2, "pickable");

    //player entity
    mPlayerID = 3;
    addEntity("pawn_hd.obj", "PawnMat", {1.5f, 0.01f, 2.5f}, {0.3f, 0.3f, 0.3f});
    resourceMgr.setTag(3, "player");
    mEntities[3].bIgnoreFrustum = true;

    //camera entity
    mCameraID = 4;
    addEntity("camera.obj", "CamMat", {11.f, 10.f, 4.f}, {0.5f, 0.5f, 0.5f});
    resourceMgr.setTag(4, "camera"); resourceMgr.setTag(4, "invisible");

    //door
    addEntity("door.obj", "WoodMat", {3.f, 0.01f, 8.5f}, {0.6f, 0.6f, 0.6f});
    resourceMgr.setTag(5, "door");

    //pressure plate
    addEntity("cube", "WoodMat", {6.5, -0.01f, 1.5f}, {0.5f, 0.1f, 0.5f});
    resourceMgr.setTag(6, "button");

    //walls
    addEntity("box.obj", "Brick", {-0.5f, 0.f, 0.25f}, {0.2f, 1.5f, 1.f}); addEntity("box.obj", "Brick", {-0.5f, 0.f, 2.25f}, {0.2f, 1.5f, 1.f}); addEntity("box.obj", "Brick", {-0.5f, 0.f, 4.25f}, {0.2f, 1.5f, 1.f}); addEntity("box.obj", "Brick", {-0.5f, 0.f, 6.25f}, {0.2f, 1.5f, 1.f}); addEntity("box.obj", "Brick", {-0.5f, 0.f, 8.25f}, {0.2f, 1.5f, 1.f});
    addEntity("box.obj", "Brick", {0.f, 0.f, -0.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {2.f, 0.f, -0.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {4.f, 0.f, -0.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {6.f, 0.f, -0.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {8.f, 0.f, -0.5f}, {1.0f, 1.5f, 0.2f});
    addEntity("box.obj", "Brick", {8.5f, 0.f, 0.25f}, {0.2f, 1.5f, 1.0f}); addEntity("box.obj", "Brick", {8.5f, 0.f, 2.25f}, {0.2f, 1.5f, 1.0f}); addEntity("box.obj", "Brick", {8.5f, 0.f, 4.25f}, {0.2f, 1.5f, 1.0f}); addEntity("box.obj", "Brick", {8.5f, 0.f, 6.25f}, {0.2f, 1.5f, 1.0f}); addEntity("box.obj", "Brick", {8.5f, 0.f, 8.25f}, {0.2f, 1.5f, 1.0f});
    addEntity("box.obj", "Brick", {1.f, 0.f, 8.5f}, {1.7f, 1.5f, 0.2f});
    addEntity("box.obj", "Brick", {6.6f, 0.f, 8.5f}, {2.f, 1.5f, 0.2f});

    //props
    addEntity("box.obj", "WoodMat", {1.f, 0.f, 0.6f}, {1.3f, 0.3f, 0.8f});
    resourceMgr.setTag(24, "solidprop");
    addEntity("plane", "PropMat", {0.2f, 0.61f, 0.6f}, {0.5f, 1.0f, 0.7f});

    //room 2
    addEntity("plane", "Metal", {4.f, 0.01f, 13.f}, {4.5f, 1.f, 4.5f});
    addEntity("plane", "Metal", {13.f, 0.01f, 13.f}, {4.5f, 1.f, 4.5f});

    addEntity("pawn_hd.obj", "PawnMat", {6.5f, 0.01f, 15.5f}, {0.3f, 0.3f, 0.3f});
    mEntities[28].mTransform.rotation.y = -90;
    mEnemyIDs.push_back(28);

    addEntity("pawn_hd.obj", "PawnMat", {12.5f, 0.01f, 9.5f}, {0.3f, 0.3f, 0.3f});
    mEntities[29].mTransform.rotation.y = 90;
    mEnemyIDs.push_back(29);

    addEntity("box.obj", "Brick", {-0.5f, 0.f, 10.25f}, {0.2f, 1.5f, 1.f}); addEntity("box.obj", "Brick", {-0.5f, 0.f, 12.25f}, {0.2f, 1.5f, 1.f}); addEntity("box.obj", "Brick", {-0.5f, 0.f, 14.25f}, {0.2f, 1.5f, 1.f}); addEntity("box.obj", "Brick", {-0.5f, 0.f, 16.25f}, {0.2f, 1.5f, 1.f});
    addEntity("box.obj", "Brick", {0.25f, 0.f, 17.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {2.25f, 0.f, 17.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {4.25f, 0.f, 17.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {6.25f, 0.f, 17.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {8.25f, 0.f, 17.5f}, {1.0f, 1.5f, 0.2f});
    addEntity("box.obj", "Brick", {10.25f, 0.f, 17.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {12.25f, 0.f, 17.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {14.25f, 0.f, 17.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {16.25f, 0.f, 17.5f}, {1.0f, 1.5f, 0.2f});
    addEntity("box.obj", "Brick", {17.5f, 0.f, 8.75f}, {0.2f, 1.5f, 1.0f}); addEntity("box.obj", "Brick", {17.5f, 0.f, 10.75f}, {0.2f, 1.5f, 1.0f}); addEntity("box.obj", "Brick", {17.5f, 0.f, 12.75f}, {0.2f, 1.5f, 1.0f}); addEntity("box.obj", "Brick", {17.5f, 0.f, 14.75f}, {0.2f, 1.5f, 1.0f}); addEntity("box.obj", "Brick", {17.5f, 0.f, 16.75f}, {0.2f, 1.5f, 1.0f});

    //detection boxes
    addEntity("box.obj", "Outline", mEntities[mEnemyIDs[0]].mTransform.position, {2.5f, 0.1f, 2.5f});
    resourceMgr.setTag(48, "detector"); resourceMgr.setTag(48, "invisible");
    resourceMgr.setOBBCollider(48, mEntities[48].mTransform.position - mEntities[48].mTransform.scale, mEntities[48].mTransform.position + mEntities[48].mTransform.scale, true);
    mEntities[48].bHidden = true;

    addEntity("box.obj", "Outline", mEntities[mEnemyIDs[1]].mTransform.position, {2.5f, 0.1f, 2.5f});
    resourceMgr.setTag(49, "detector"); resourceMgr.setTag(49, "invisible");
    resourceMgr.setOBBCollider(49, mEntities[49].mTransform.position - mEntities[49].mTransform.scale, mEntities[49].mTransform.position + mEntities[49].mTransform.scale, true);
    mEntities[49].bHidden = true;

    //health bar
    addEntity("plane", "HPbar", mEntities[mPlayerID].mTransform.position + gsl::vec3{0.f, 2.5f, 0.f}, {0.2f, 1.f, 1.f});
    resourceMgr.setTag(50, "healthbar"); resourceMgr.setTag(50, "hud");

    //additional floors and walls
    addEntity("plane", "Metal", {13.f, 0.01f, 4.f}, {4.5f, 1.f, 4.5f});
    addEntity("box.obj", "Brick", {10.f, 0.f, -0.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {12.f, 0.f, -0.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {14.f, 0.f, -0.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {16.f, 0.f, -0.5f}, {1.0f, 1.5f, 0.2f}); addEntity("box.obj", "Brick", {18.f, 0.f, -0.5f}, {1.0f, 1.5f, 0.2f});

    //key trophy
    addEntity("key.obj", "CamMat", {13.f, 0.5f, 4.f}, {0.5f, 0.5f, 0.5f});
    mKeyID = 57;
    resourceMgr.setTag(57, "pickable");

    //mini map
    addEntity("plane", "map", mEntities[mCameraID].mTransform.position - gsl::vec3{1.2f, 0.9f, 0.75f}, {0.1f, 0.1f, 0.1f});
    resourceMgr.setTag(58, "map"); resourceMgr.setTag(58, "hud");
    mEntities[58].bIgnoreFrustum = true;

    //game over screen
    addEntity("plane", "goscreen", mEntities[mCameraID].mTransform.position - gsl::vec3{1.0f, 1.5f, 0.f}, {0.5f, 0.5f, 0.5f});
    resourceMgr.setTag(59, "goscreen");
    mEntities[59].bHidden = true;
    mEntities[59].bIgnoreFrustum = true;

    //tips
    addEntity("plane", "tips", mEntities[mCameraID].mTransform.position - gsl::vec3{1.1f, 1.0f, -0.7f}, {0.2f, 0.2f, 0.2f});
    resourceMgr.setTag(60, "tip"); resourceMgr.setTag(60, "hud");

    //setting components
    mEntities[mCameraID].mTransform.rotation = {0, 180, 55};

    resourceMgr.setOBBCollider(2, mEntities[2].mTransform.position - mEntities[2].mTransform.scale, mEntities[2].mTransform.position + mEntities[2].mTransform.scale, true);
    resourceMgr.setOBBCollider(mPlayerID, mEntities[mPlayerID].mTransform.position - mEntities[mPlayerID].mTransform.position, mEntities[mPlayerID].mTransform.position + mEntities[mPlayerID].mTransform.scale, true);
    resourceMgr.setOBBCollider(5, mEntities[5].mTransform.position - gsl::vec3{0.3f, 2.f, 0.2}, mEntities[5].mTransform.position + gsl::vec3{1.6f, 2.f, 0.2}, false);
    resourceMgr.setOBBCollider(6, mEntities[6].mTransform.position - mEntities[6].mTransform.scale, mEntities[6].mTransform.position + mEntities[6].mTransform.scale, false);
    resourceMgr.setOBBCollider(12, mEntities[12].mTransform.position - mEntities[12].mTransform.scale, mEntities[12].mTransform.position + mEntities[12].mTransform.scale, false);
    resourceMgr.setOBBCollider(24, mEntities[24].mTransform.position - mEntities[24].mTransform.scale, mEntities[24].mTransform.position + mEntities[24].mTransform.scale, false);
    resourceMgr.setOBBCollider(mKeyID, mEntities[mKeyID].mTransform.position - mEntities[mKeyID].mTransform.scale, mEntities[mKeyID].mTransform.position + mEntities[mKeyID].mTransform.scale, true);

    resourceMgr.setSound("pop", 2, gsl::SoundFilePath + "Pop.wav", false, 1.f);
    resourceMgr.setSound("open", 6, gsl::SoundFilePath + "DoorOpen.wav", false, 1.f);
    resourceMgr.setSound("close", 5, gsl::SoundFilePath + "DoorClose.wav", false, 1.f);
    resourceMgr.setSound("bonk", 48, gsl::SoundFilePath + "Bonk.wav", false, 1.f);
    resourceMgr.setSound("bonk", 49, gsl::SoundFilePath + "Bonk.wav", false, 1.f);
    resourceMgr.setSound("steps", mEnemyIDs[0], gsl::SoundFilePath + "Footsteps.wav", true, 0.5f);
    resourceMgr.setSound("steps", mEnemyIDs[1], gsl::SoundFilePath + "Footsteps.wav", true, 0.5f);
    resourceMgr.setSound("soundtrack", mPlayerID, gsl::SoundFilePath + "SneakySnitch.wav", true, 1.f);

    resourceMgr.setScript(mPlayerID, "PlayerData.js");

    for (MAX_ENTITIES_TYPE i{7}; i<24; i++)
    {
        resourceMgr.setTag(i, "wall");
        resourceMgr.setOBBCollider(i, mEntities[i].mTransform.position - mEntities[i].mTransform.scale, mEntities[i].mTransform.position + mEntities[i].mTransform.scale, true);
    }

    for (MAX_ENTITIES_TYPE i{30}; i<48; i++)
    {
        resourceMgr.setTag(i, "wall");
        resourceMgr.setOBBCollider(i, mEntities[i].mTransform.position - mEntities[i].mTransform.scale, mEntities[i].mTransform.position + mEntities[i].mTransform.scale, true);
    }

    for (MAX_ENTITIES_TYPE i{52}; i<57; i++)
    {
        resourceMgr.setTag(i, "wall");
        resourceMgr.setOBBCollider(i, mEntities[i].mTransform.position - mEntities[i].mTransform.scale, mEntities[i].mTransform.position + mEntities[i].mTransform.scale, true);
    }

    for (MAX_ENTITIES_TYPE i{0}; i<mEnemyIDs.size(); i++)
    {
        resourceMgr.setOBBCollider(mEnemyIDs[i], mEntities[mEnemyIDs[i]].mTransform.position - mEntities[mEnemyIDs[i]].mTransform.scale, mEntities[mEnemyIDs[i]].mTransform.position + mEntities[mEnemyIDs[i]].mTransform.scale, true);
        resourceMgr.setScript(mEnemyIDs[i], "EnemyData.js");
    }

    mEntities[mPlayerID].mCurrentHP = mEntities[mPlayerID].mScript.pHP;
}

void Scene::update()
{
    //won't do anything if the game is not being played
    if (!ResourceManager::getInstance().mCoreEngine->bTogglePlay)
    {
        mEntities[mPlayerID].bHidden = false;
        return;
    }
    //update collider position
    ResourceManager::getInstance().updateColliders();

    //update position of picked objects
    for (MAX_ENTITIES_TYPE i{0}; i<mEntities.size(); i++)
    {
        if (mEntities[i].bPicked)
        {
            if (mEntities[mPlayerID].mTransform.rotation.y == 0)
                mEntities[i].mTransform.position = mEntities[mPlayerID].mTransform.position + gsl::vec3{0.7f, 1.f, 0.f};
            if (mEntities[mPlayerID].mTransform.rotation.y == 90)
                mEntities[i].mTransform.position = mEntities[mPlayerID].mTransform.position + gsl::vec3{0.f, 1.f, 0.7f};
            if (mEntities[mPlayerID].mTransform.rotation.y == -90)
                mEntities[i].mTransform.position = mEntities[mPlayerID].mTransform.position + gsl::vec3{0.f, 1.f, -0.7f};
            if (mEntities[mPlayerID].mTransform.rotation.y == 180)
                mEntities[i].mTransform.position = mEntities[mPlayerID].mTransform.position + gsl::vec3{-0.7f, 1.f, 0.f};
        }
    }

    //updates HUD elements position and rotation
    for (MAX_ENTITIES_TYPE i{0}; i<mEntities.size(); i++)
    {
        for (unsigned short j{0}; j<mEntities[i].mTags.size(); j++)
        {
            if (mEntities[i].mTags[j] == "healthbar")
            {
                mEntities[i].mTransform.position = mEntities[mPlayerID].mTransform.position + gsl::vec3{0.f, 2.5f, 0.f};
                mEntities[i].mTransform.rotation.z = mEntities[mCameraID].mTransform.rotation.z/1.5;
                mEntities[i].mTransform.rotation.y = (180-mEntities[mCameraID].mTransform.rotation.y)/2;
            }
            if (mEntities[i].mTags[j] == "map")
            {
                mEntities[i].mTransform.position = mEntities[mCameraID].mTransform.position - gsl::vec3{1.2f, 0.9f, 0.75f};
                mEntities[i].mTransform.rotation.z = mEntities[mCameraID].mTransform.rotation.z/1.5;
                mEntities[i].mTransform.rotation.y = (180-mEntities[mCameraID].mTransform.rotation.y)/2;
            }
            if (mEntities[i].mTags[j] == "goscreen")
            {
                mEntities[i].mTransform.position = mEntities[mCameraID].mTransform.position - gsl::vec3{1.0f, 1.5f, 0.f};
                mEntities[i].mTransform.rotation.z = mEntities[mCameraID].mTransform.rotation.z/1.5;
                mEntities[i].mTransform.rotation.y = (180-mEntities[mCameraID].mTransform.rotation.y)/2;
            }
            if (mEntities[i].mTags[j] == "tip")
            {
                mEntities[i].mTransform.position = mEntities[mCameraID].mTransform.position - gsl::vec3{1.1f, 1.0f, -0.7f};
                mEntities[i].mTransform.rotation.z = mEntities[mCameraID].mTransform.rotation.z/1.5;
                mEntities[i].mTransform.rotation.y = (180-mEntities[mCameraID].mTransform.rotation.y)/2;
            }
        }
    }

    //tag-based collision tests
    for (MAX_ENTITIES_TYPE i{0}; i < mEntities.size(); i++)
    {

        for (unsigned short j{0}; j < mEntities[i].mTags.size(); j++)
        {
            if (mEntities[i].mTags[j] == "button")
            {
                //opens door on collision with pressure plate cube
                if(ResourceManager::getInstance().testCollision(mEntities[2], mEntities[i]) || ResourceManager::getInstance().testCollision(mEntities[mPlayerID], mEntities[i]))
                {
                    if (!mEntities[5].bOpened)
                    {
                        if (mRenderWindow->bToggleSound) ResourceManager::getInstance().playSound(6);
                        mEntities[5].bOpened = true;
                    }
                    mEntities[5].mTransform.rotation.y = 90;
                }
                else
                {
                    if (mEntities[5].bOpened)
                    {
                        if (mRenderWindow->bToggleSound) ResourceManager::getInstance().playSound(5);
                        mEntities[5].bOpened = false;
                    }
                    mEntities[5].mTransform.rotation.y = 0;
                }
            }

            if (mEntities[i].mTags[j] == "detector")
            {
                if(ResourceManager::getInstance().testCollision(mEntities[mPlayerID], mEntities[i]))
                {
                    //puts player back to jail when collided by enemy detection box
                    if (mRenderWindow->bToggleSound)
                        ResourceManager::getInstance().playSound(i);

                    mEntities[mPlayerID].mTransform.position = mEntities[mPlayerID].mInitPos;
                    if (i == 48) mEntities[mPlayerID].mCurrentHP -= mEntities[mEnemyIDs[0]].mScript.pStr;
                    if (i == 49) mEntities[mPlayerID].mCurrentHP -= mEntities[mEnemyIDs[1]].mScript.pStr;
                }
            }
        }
    }

    //simple LOD test
    if (mRenderWindow->bToggleLOD)
    {
        gsl::vec3 distanceLOD = mEntities[mCameraID].mTransform.position - mEntities[mPlayerID].mTransform.position;
        if (distanceLOD.length() < 15.f) ResourceManager::getInstance().setMesh(mPlayerID, "pawn_hd.obj");
        else ResourceManager::getInstance().setMesh(mPlayerID, "pawn_ld.obj");

        distanceLOD = mEntities[mCameraID].mTransform.position - mEntities[mEnemyIDs[0]].mTransform.position;
        if (distanceLOD.length() < 15.f) ResourceManager::getInstance().setMesh(mEnemyIDs[0], "pawn_hd.obj");
        else ResourceManager::getInstance().setMesh(mEnemyIDs[0], "pawn_ld.obj");

        distanceLOD = mEntities[mCameraID].mTransform.position - mEntities[mEnemyIDs[1]].mTransform.position;
        if (distanceLOD.length() < 15.f) ResourceManager::getInstance().setMesh(mEnemyIDs[1], "pawn_hd.obj");
        else ResourceManager::getInstance().setMesh(mEnemyIDs[1], "pawn_ld.obj");
    }

    //key animation
    if (!mEntities[mKeyID].bPicked)
        mEntities[mKeyID].mTransform.rotation.y += 0.5;
    else
        mEntities[mKeyID].mTransform.rotation.y = 0;

    //enemy patrols
    if (!mEntities[mEnemyIDs[0]].bMoveUp)
    {
        mEntities[mEnemyIDs[0]].mTransform.position.x += 0.02f * mEntities[mEnemyIDs[0]].mScript.pSpeed;
        mEntities[mEnemyIDs[0]].mTransform.rotation.y = 0;
        mEntities[48].mTransform.position = mEntities[mEnemyIDs[0]].mTransform.position + gsl::vec3{1.5f, 0.f, 0.f};
        if (mEntities[mEnemyIDs[0]].mTransform.position.x >= 15.f) mEntities[mEnemyIDs[0]].bMoveUp = true;
    }
    else
    {
        mEntities[mEnemyIDs[0]].mTransform.position.x -= 0.02f * mEntities[mEnemyIDs[0]].mScript.pSpeed;
        mEntities[mEnemyIDs[0]].mTransform.rotation.y = 180;
        mEntities[48].mTransform.position = mEntities[mEnemyIDs[0]].mTransform.position - gsl::vec3{1.5f, 0.f, 0.f};
        if (mEntities[mEnemyIDs[0]].mTransform.position.x <= 1.f) mEntities[mEnemyIDs[0]].bMoveUp = false;
    }

    if (!mEntities[mEnemyIDs[1]].bMoveUp)
    {
        mEntities[mEnemyIDs[1]].mTransform.position.z += 0.02f * mEntities[mEnemyIDs[1]].mScript.pSpeed;
        mEntities[mEnemyIDs[1]].mTransform.rotation.y = 90;
        mEntities[49].mTransform.position = mEntities[mEnemyIDs[1]].mTransform.position + gsl::vec3{0.f, 0.f, 1.5f};
        if (mEntities[mEnemyIDs[1]].mTransform.position.z >= 15.f) mEntities[mEnemyIDs[1]].bMoveUp = true;
    }
    else
    {
        mEntities[mEnemyIDs[1]].mTransform.position.z -= 0.02f * mEntities[mEnemyIDs[1]].mScript.pSpeed;
        mEntities[mEnemyIDs[1]].mTransform.rotation.y = -90;
        mEntities[49].mTransform.position = mEntities[mEnemyIDs[1]].mTransform.position - gsl::vec3{0.f, 0.f, 1.5f};
        if (mEntities[mEnemyIDs[1]].mTransform.position.z <= 1.f) mEntities[mEnemyIDs[1]].bMoveUp = false;
    }

    //update player health bar
    if (mEntities[mPlayerID].mCurrentHP <= 100)
    {
        ResourceManager::getInstance().mMaterial[mEntities[50].mMaterialIDIndex]->mTextureId = ResourceManager::getInstance().mTexture[17]->id();
        mEntities[59].bHidden = true;
        mEntities[mPlayerID].bHidden = false;
    }
    if (mEntities[mPlayerID].mCurrentHP <= 90)
        ResourceManager::getInstance().mMaterial[mEntities[50].mMaterialIDIndex]->mTextureId = ResourceManager::getInstance().mTexture[16]->id();
    if (mEntities[mPlayerID].mCurrentHP <= 80)
        ResourceManager::getInstance().mMaterial[mEntities[50].mMaterialIDIndex]->mTextureId = ResourceManager::getInstance().mTexture[15]->id();
    if (mEntities[mPlayerID].mCurrentHP <= 70)
        ResourceManager::getInstance().mMaterial[mEntities[50].mMaterialIDIndex]->mTextureId = ResourceManager::getInstance().mTexture[14]->id();
    if (mEntities[mPlayerID].mCurrentHP <= 60)
        ResourceManager::getInstance().mMaterial[mEntities[50].mMaterialIDIndex]->mTextureId = ResourceManager::getInstance().mTexture[13]->id();
    if (mEntities[mPlayerID].mCurrentHP <= 50)
        ResourceManager::getInstance().mMaterial[mEntities[50].mMaterialIDIndex]->mTextureId = ResourceManager::getInstance().mTexture[12]->id();
    if (mEntities[mPlayerID].mCurrentHP <= 40)
        ResourceManager::getInstance().mMaterial[mEntities[50].mMaterialIDIndex]->mTextureId = ResourceManager::getInstance().mTexture[11]->id();
    if (mEntities[mPlayerID].mCurrentHP <= 30)
        ResourceManager::getInstance().mMaterial[mEntities[50].mMaterialIDIndex]->mTextureId = ResourceManager::getInstance().mTexture[10]->id();
    if (mEntities[mPlayerID].mCurrentHP <= 20)
        ResourceManager::getInstance().mMaterial[mEntities[50].mMaterialIDIndex]->mTextureId = ResourceManager::getInstance().mTexture[9]->id();
    if (mEntities[mPlayerID].mCurrentHP <= 10)
        ResourceManager::getInstance().mMaterial[mEntities[50].mMaterialIDIndex]->mTextureId = ResourceManager::getInstance().mTexture[8]->id();
    if (mEntities[mPlayerID].mCurrentHP <= 0)
    {
        //game lost
        ResourceManager::getInstance().stopAllSound();
        ResourceManager::getInstance().mMaterial[mEntities[59].mMaterialIDIndex]->mTextureId = ResourceManager::getInstance().mTexture[19]->id();
        mEntities[59].bHidden = false;
        mEntities[mPlayerID].bHidden = true;
    }

    if (mEntities[mPlayerID].mTransform.position.x >= 18.f && mEntities[mKeyID].bPicked && mEntities[mPlayerID].mCurrentHP > 0)
    {
        //game won
        ResourceManager::getInstance().stopAllSound();
        ResourceManager::getInstance().mMaterial[mEntities[59].mMaterialIDIndex]->mTextureId = ResourceManager::getInstance().mTexture[20]->id();
        mEntities[59].bHidden = false;
        mEntities[mPlayerID].bHidden = true;
    }
}


//collision test between entity and walls
void Scene::testWallCollision(MAX_ENTITIES_TYPE entityID, char dir)
{
    for (MAX_ENTITIES_TYPE i{0}; i<mEntities.size(); i++)
    {
        for (MAX_ENTITIES_TYPE j{0}; j<mEntities[i].mTags.size(); j++)
        {
            if (mEntities[i].mTags[j] == "wall" || mEntities[i].mTags[j] == "door" || mEntities[i].mTags[j] == "solidprop")
            {
                if (ResourceManager::getInstance().testCollision(mEntities[entityID], mEntities[i]))
                {
                    if (mEntities[i].mTags[j] == "door" && mEntities[i].mTransform.rotation.y != 0) return;

                    if (dir == 'f' && mEntities[entityID].mTransform.position.x > mEntities[i].mTransform.position.x)
                        mEntities[entityID].mTransform.position.x = mEntities[i].mCollider.mMaxRadius.x + mEntities[entityID].mTransform.scale.x + 0.02;
                    if (dir == 'b' && mEntities[entityID].mTransform.position.x < mEntities[i].mTransform.position.x)
                        mEntities[entityID].mTransform.position.x = mEntities[i].mCollider.mMinCenter.x - mEntities[entityID].mTransform.scale.x - 0.02;
                    if (dir == 'r' && mEntities[entityID].mTransform.position.z > mEntities[i].mTransform.position.z)
                        mEntities[entityID].mTransform.position.z = mEntities[i].mCollider.mMaxRadius.z + mEntities[entityID].mTransform.scale.z + 0.02;
                    if (dir == 'l' && mEntities[entityID].mTransform.position.z < mEntities[i].mTransform.position.z)
                        mEntities[entityID].mTransform.position.z = mEntities[i].mCollider.mMinCenter.z - mEntities[entityID].mTransform.scale.z - 0.02;
                }
            }
        }
    }
}

//simple collision test, lets player pick up entity on collision
void Scene::pickObject()
{
    for (MAX_ENTITIES_TYPE i{0}; i < mEntities.size(); i++)
    {
        for (unsigned short j{0}; j < mEntities[i].mTags.size(); j++)
        {
            if (mEntities[i].mTags[j] == "pickable")
            {
                if (!mEntities[i].bPicked)
                {
                    if(mEntities[i].bCollidable && ResourceManager::getInstance().testCollision(mEntities[mPlayerID], mEntities[i]))
                    {
                        mEntities[i].bPicked = true;
                        if (mRenderWindow->bToggleSound) ResourceManager::getInstance().playSound(2);
                    }
                }
                else
                {
                    mEntities[i].bPicked = false;
                    mEntities[i].mTransform.position.y = mEntities[i].mInitPos.y;
                }
            }
        }
    }

}

void Scene::reset()
{
    for (MAX_ENTITIES_TYPE i{0}; i < mEntities.size(); i++)
    {
        mEntities[i].mTransform.position = mEntities[i].mInitPos;
        mEntities[i].bPicked = false;
        mEntities[i].bHidden = false;

        for (unsigned int j{0}; j < mEntities[i].mTags.size(); j++)
            if (mEntities[i].mTags[j] == "invisible") mEntities[i].bHidden = true;
    }
    mEntities[mPlayerID].mCurrentHP = mEntities[mPlayerID].mScript.pHP;
}

void Scene::render()
{
    mRenderSystem->Render();
    mRenderWindow->checkForGLerrors();
}

void Scene::renderNormals(GLint matrixUniform)
{
    mRenderSystem->renderNormals(matrixUniform);
}

void Scene::renderMousepicker(GLint matrixUniform, GLint colorUniform)
{
    mRenderSystem->renderMousepicker(matrixUniform, colorUniform);
}

void Scene::prepareComponentArrays()
{
    mRenderSystem->mNumberOfMeshes = mEntityCount;
}

MAX_ENTITIES_TYPE Scene::getNewEntityID()
{
    if(mEntityCount == MAX_ENTITIES) {
        mEntityCount = 1;
    }
    else {
        mEntityCount++;
    }
    return mEntityCount - 1;    //id is one less than number
}

void Scene::addEntity(std::string name, std::string mat, gsl::vec3 pos, gsl::vec3 scale)
{
    ResourceManager &resourceMgr = ResourceManager::getInstance();
    MAX_ENTITIES_TYPE tempEnt;
    ecs::Transform mTransform;

    mTransform.position = pos;
    mTransform.scale = scale;
    tempEnt = resourceMgr.make3DEntity(name, mTransform, mat);
}

void Scene::lightSetup(std::string name, std::string mat, gsl::vec3 pos, gsl::vec3 scale)
{
    //set up light
    ResourceManager &resourceMgr = ResourceManager::getInstance();
    MAX_ENTITIES_TYPE tempEnt;
    ecs::Light tempLight;
    ecs::Transform mTransform;

    mTransform.position = pos;
    mTransform.scale = scale;
    tempLight.mAmbientStrength = 0.5f;
    tempLight.mLightStrength = 2.f;
    tempLight.mLightColour = gsl::vec3{0.2f, 0.3f, 0.5f};
    tempEnt = resourceMgr.makeLightEntity(name, mTransform, mat, &tempLight);
    mEntities[tempEnt].mLight = tempLight;
}

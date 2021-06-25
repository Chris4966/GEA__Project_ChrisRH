#include "resourcemanager.h"

#include <utility>
#include <QDir>
#include <QDebug>
#include <string>
#include "texture.h"
#include "phongshader.h"
#include "textureshader.h"
#include "colorshader.h"
#include "normalsshader.h"
#include "skyboxshader.h"
#include "hudshader.h"
#include "mousepickershader.h"
#include "singlecolorshader.h"
#include "meshobject.h"
#include "coreengine.h"
#include "gslpch.h"
#include "vertex.h"
#include "scriptsystem.h"

ResourceManager::ResourceManager() : mDevice(NULL), mContext(NULL)
{

}

bool ResourceManager::searchMap(std::string &name, GLuint &index)
{
    auto result = mMeshObjectMap.find(name);
    if (result != mMeshObjectMap.end()) {
        index = result->second;
        return true;
    }
    else
        return false;
}

ResourceManager &ResourceManager::getInstance()
{
    static ResourceManager *mInstance = new ResourceManager();
    return *mInstance;
}

void ResourceManager::readTextures()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    QDir tempDir((gsl::TextureFilePath).c_str());
    if(tempDir.exists())
    {
        QStringList filters;
        filters << "*.bmp";
        tempDir.setNameFilters(filters);
        qDebug() << "\n" << tempDir.entryInfoList().size() << "textures will be read out of"
                 << gsl::NumberOfTextures << "texture places in Engines texture array! \n" ;

        //        Q_ASSERT(gsl::NumberOfTextures < tempDir.entryInfoList().size() &&
        //                 "More textures in directory than gsl::NumberOfTextures");

        mIndexOfLastTexture++;
        mTexture[mIndexOfLastTexture] = new Texture(); //default texture

        //read all regular textures
        for(QFileInfo var : tempDir.entryInfoList())
        {
            qDebug() << "Texture name: " << var.fileName();
            Texture *temp = new Texture(var.fileName().toStdString(), false);
            mIndexOfLastTexture++;
            mTexture[mIndexOfLastTexture] = temp;
        }
    }
    else
    {
        qDebug() << "*** ERROR reading textures *** : Asset-folder " << gsl::AssetFilePath.c_str() << " does not exist!";
    }

    QDir tempDir2((gsl::AssetFilePath + "CubeMaps").c_str());
    if(tempDir2.exists())
    {
        QStringList filters;
        filters << "*1.bmp";
        tempDir2.setNameFilters(filters);
        qDebug() << tempDir2.entryInfoList().size();
        //read all cubemaps
        for(QFileInfo var : tempDir2.entryInfoList())
        {
            if(var.baseName().endsWith("0001"))
            {
                Texture *temp = new Texture(var.fileName().toStdString(), true);
                mIndexOfLastTexture++;
                mTexture[mIndexOfLastTexture] = temp;
            }
        }
    }
    else
    {
        qDebug() << "*** ERROR reading Cubemap textures *** : Asset-folder " << tempDir2.dirName() << " does not exist!";
    }
    for(int i = 0; i <= mIndexOfLastTexture;++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        if(!mTexture[i]->mCubemap[0])
            glBindTexture(GL_TEXTURE_2D, mTexture[i]->id());
        else
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture[i]->id());  //Skybox
            qDebug() << "One cubemap made";
        }
    }
}

void ResourceManager::readObjs()
{
    QDir tempDir((gsl::MeshFilePath).c_str());
    if(tempDir.exists())
    {
        QStringList filters;
        filters << "*.obj";
        tempDir.setNameFilters(filters);
        qDebug() << "\n" <<  tempDir.entryInfoList().size() << " obj-s will be read\n";

        //read all regular obj´s
        for(QFileInfo var : tempDir.entryInfoList())
        {
            qDebug() << "obj name: " << var.fileName();
            mMeshObjects.emplace_back(new MeshObject(gsl::AssetFilePath + "Meshes/" + var.fileName().toStdString()));
            unsigned int indexToMesh = mMeshObjects.size()-1;
            mMeshObjectMap.emplace(var.fileName().toStdString(), indexToMesh);
        }
    }
    else
    {
        qDebug() << "*** ERROR reading obj´s *** : Asset-folder " << tempDir.dirName()  << " does not exist!";
    }
}

void ResourceManager::readShaders(RenderWindow *renderWindow)
{
    mShaderProgram[gsl::COLORSHADER] = new ColorShader("colorshader");
    mShaderProgram[gsl::COLORSHADER]->mRenderWindow = renderWindow;  //only for debugging
    mShaderProgram[gsl::COLORSHADER]->setupShader();

    mShaderProgram[gsl::TEXTURESHADER] = new TextureShader("textureshader");
    mShaderProgram[gsl::TEXTURESHADER]->mRenderWindow = renderWindow;  //only for debugging
    mShaderProgram[gsl::TEXTURESHADER]->setupShader();

    mShaderProgram[gsl::PHONGSHADER] = new PhongShader("phongshader");
    mShaderProgram[gsl::PHONGSHADER]->mRenderWindow = renderWindow; //only for debugging
    mShaderProgram[gsl::PHONGSHADER]->setupShader();

    mShaderProgram[gsl::NORMALSSHADER] = new NormalsShader("shownormalsshader");
    mShaderProgram[gsl::NORMALSSHADER]->mRenderWindow = renderWindow; //only for debugging
    mShaderProgram[gsl::NORMALSSHADER]->setupShader();

    mShaderProgram[gsl::SKYBOXSHADER] = new SkyboxShader("skyboxshader");
    mShaderProgram[gsl::SKYBOXSHADER]->mRenderWindow = renderWindow; //only for debugging
    mShaderProgram[gsl::SKYBOXSHADER]->setupShader();

    mShaderProgram[gsl::MOUSEPICKERSHADER] = new MousePickerShader("mousepickershader");
    mShaderProgram[gsl::MOUSEPICKERSHADER]->mRenderWindow = renderWindow; //only for debugging
    mShaderProgram[gsl::MOUSEPICKERSHADER]->setupShader();

    mShaderProgram[gsl::SINGLECOLORSHADER] = new SingleColorShader("singlecolorshader");
    mShaderProgram[gsl::SINGLECOLORSHADER]->mRenderWindow = renderWindow; //only for debugging
    mShaderProgram[gsl::SINGLECOLORSHADER]->setupShader();

    mShaderProgram[gsl::HUDSHADER] = new HUDShader("hudshader");
    mShaderProgram[gsl::HUDSHADER]->mRenderWindow = renderWindow;  //only for debugging
    mShaderProgram[gsl::HUDSHADER]->setupShader();
}

void ResourceManager::makeMaterials()
{
    ecs::Material *tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::COLORSHADER;
    tempMaterial->mColor = gsl::vec3{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 0;
    tempMaterial->mMatName = "PlainGray";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::TEXTURESHADER;
    tempMaterial->mTextureId = mTexture[1]->id();
    tempMaterial->mColor = gsl::vec3{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 1;
    tempMaterial->mMatName = "TextureA";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::PHONGSHADER;
    tempMaterial->mTextureId = mTexture[3]->id();
    tempMaterial->mColor = gsl::vec3{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 2;
    tempMaterial->mMatName = "Shiny";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    tempMaterial->mSpecularStrenght = 0.9f;
    tempMaterial->mSpecularExponent = 164;
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::PHONGSHADER;
    tempMaterial->mTextureId = mTexture[3]->id();
    tempMaterial->mColor = gsl::vec3{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 3;
    tempMaterial->mMatName = "Dull";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    tempMaterial->mSpecularStrenght = 0.1f;
    tempMaterial->mSpecularExponent = 1;
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::SINGLECOLORSHADER;
    tempMaterial->mColor = gsl::vec3{1.0f, 1.0f, 0.f};
    tempMaterial->mMaterialID = 4;
    tempMaterial->mMatName = "Outline";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::HUDSHADER;
    tempMaterial->mMaterialID = 5;
    tempMaterial->mMatName = "HUDelements";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::SINGLECOLORSHADER;
    tempMaterial->mColor = gsl::vec3{0.1f, 0.1f, 0.1f};
    tempMaterial->mMaterialID = 6;
    tempMaterial->mMatName = "CamMat";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::PHONGSHADER;
    tempMaterial->mTextureId = mTexture[6]->id();
    tempMaterial->mColor = gsl::vec3{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 7;
    tempMaterial->mMatName = "WoodMat";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    tempMaterial->mSpecularStrenght = 0.4f;
    tempMaterial->mSpecularExponent = 24;
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::PHONGSHADER;
    tempMaterial->mTextureId = mTexture[4]->id();
    tempMaterial->mColor = gsl::vec3{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 8;
    tempMaterial->mMatName = "Metal";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    tempMaterial->mSpecularStrenght = 0.6f;
    tempMaterial->mSpecularExponent = 64;
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::PHONGSHADER;
    tempMaterial->mTextureId = mTexture[7]->id();
    tempMaterial->mColor = gsl::vec3{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 9;
    tempMaterial->mMatName = "PawnMat";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    tempMaterial->mSpecularStrenght = 0.5f;
    tempMaterial->mSpecularExponent = 32;
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::PHONGSHADER;
    tempMaterial->mTextureId = mTexture[5]->id();
    tempMaterial->mColor = gsl::vec3{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 10;
    tempMaterial->mMatName = "Brick";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    tempMaterial->mSpecularStrenght = 0.5f;
    tempMaterial->mSpecularExponent = 32;
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::SINGLECOLORSHADER;
    tempMaterial->mColor = gsl::vec3{0.1f, 0.1f, 0.1f};
    tempMaterial->mMaterialID = 11;
    tempMaterial->mMatName = "PropMat";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::TEXTURESHADER;
    tempMaterial->mTextureId = mTexture[17]->id();
    tempMaterial->mColor = gsl::vec3{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 12;
    tempMaterial->mMatName = "HPbar";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::TEXTURESHADER;
    tempMaterial->mTextureId = mTexture[18]->id();
    tempMaterial->mColor = gsl::vec3{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 13;
    tempMaterial->mMatName = "map";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::TEXTURESHADER;
    tempMaterial->mTextureId = mTexture[19]->id();
    tempMaterial->mColor = gsl::vec3{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 14;
    tempMaterial->mMatName = "goscreen";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    tempMaterial = new ecs::Material();
    tempMaterial->mShader = gsl::TEXTURESHADER;
    tempMaterial->mTextureId = mTexture[21]->id();
    tempMaterial->mColor = gsl::vec3{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 15;
    tempMaterial->mMatName = "tips";
    mMaterialNames.insert(std::pair<std::string, unsigned short>(tempMaterial->mMatName, tempMaterial->mMaterialID));
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;
}

MAX_ENTITIES_TYPE ResourceManager::make3DEntity(std::string meshName, ecs::Transform transformIn, std::string materialName)
{
    if(!mCurrentScene)
    {
        qDebug() << "ResourceManager has no current scene to make entities in!!";
        return 65535;
    }

    MAX_ENTITIES_TYPE entityID = mCurrentScene->getNewEntityID();

    mCurrentScene->mEntities.emplace_back(Entity());
    setMesh(entityID, meshName);
    mCurrentScene->mEntities[entityID].mTransform = transformIn;
    mCurrentScene->mEntities[entityID].mInitPos = transformIn.position;
    setMaterial(entityID, materialName);

    mCurrentScene->mEntities[entityID].mName = meshName + "_" + std::to_string(static_cast<int>(entityID));
    mCurrentScene->mEntities[entityID].mEntityID = entityID;

    return entityID;
}

MAX_ENTITIES_TYPE ResourceManager::makeLightEntity(std::string meshName, ecs::Transform transformIn, std::string materialName, ecs::Light *lightIn)
{
    MAX_ENTITIES_TYPE entityID = make3DEntity(meshName, transformIn, materialName);

    static_cast<PhongShader*>(mShaderProgram[gsl::PHONGSHADER])->mCurrentLight = lightIn;
    static_cast<PhongShader*>(mShaderProgram[gsl::PHONGSHADER])->mTransform = &mCurrentScene->mEntities[entityID].mTransform;

    return entityID;
}

void ResourceManager::setMesh(MAX_ENTITIES_TYPE entityID, std::string meshName)
{
    int indexToMesh{-1};
    MeshObject* temp{nullptr};

    //check if meshname already exist - if so use existing mesh
    auto result = mMeshObjectMap.find(meshName);
    if (result != mMeshObjectMap.end()) {        //found!!!
        indexToMesh = result->second;
        temp = mMeshObjects[indexToMesh];
    }
    else {
        //if meshname not found, make new MeshObject and put into mMeshObjectMap and mMeshObjects
        temp = new MeshObject(meshName);
        mMeshObjects.emplace_back(temp);
        indexToMesh = mMeshObjects.size()-1;
        mMeshObjectMap.emplace(meshName, indexToMesh);
    }
    ecs::Mesh tempMesh{temp->mMesh};

    mCurrentScene->mEntities[entityID].mMesh = tempMesh;
}

void ResourceManager::setTransform(MAX_ENTITIES_TYPE entityID, gsl::mat4 TMatrix)
{
    mCurrentScene->mEntities[entityID].mTransform.position = TMatrix.getPosition();
}

void ResourceManager::setMaterial(MAX_ENTITIES_TYPE entityID, std::string matName)
{

    int indexToMaterial{-1};
    auto resultMaterial = mMaterialNames.find(matName);
    if (resultMaterial != mMaterialNames.end()) {        //found!!!
        indexToMaterial = resultMaterial->second;
    }
    else {
        qDebug() << " ******** Error:" << matName.c_str() << "material not found - using default material!!!!!";
        indexToMaterial = 0;        //default material
    }

    mCurrentScene->mEntities[entityID].mMaterialIDIndex = indexToMaterial; //mMaterialID.mEntityID = entityID;
    mCurrentScene->mMaterialIDs[entityID].mMaterialIndex = indexToMaterial;
    mCurrentScene->mMaterialIDs[entityID].mEntityID = entityID;
}

void ResourceManager::setScript(MAX_ENTITIES_TYPE entityID, std::string fileName)
{
    ecs::Script tempScript;
    tempScript.mName = fileName;
    mCurrentScene->mEntities[entityID].mScript = tempScript;
    mCurrentScene->mEntities[entityID].bHasScript = true;
}

void ResourceManager::setTag(MAX_ENTITIES_TYPE entityID, std::string tagName)
{
    mCurrentScene->mEntities[entityID].mTags.push_back(tagName);
}

void ResourceManager::setOBBCollider(MAX_ENTITIES_TYPE entityID, gsl::vec3 min, gsl::vec3 max, bool isDynamic)
{
    mCurrentScene->mEntities[entityID].mCollider.mType = gsl::OBB;
    mCurrentScene->mEntities[entityID].mCollider.mMinCenter = min;
    mCurrentScene->mEntities[entityID].mCollider.mMaxRadius = max;
    mCurrentScene->mEntities[entityID].bCollidable = true;
    mCurrentScene->mEntities[entityID].mCollider.bIsDynamic = isDynamic;
}

void ResourceManager::setSphereCollider(MAX_ENTITIES_TYPE entityID, float r, bool isDynamic)
{
    mCurrentScene->mEntities[entityID].mCollider.mType = gsl::SPHERE;
    mCurrentScene->mEntities[entityID].mCollider.mMinCenter = mCurrentScene->mEntities[entityID].mTransform.position;
    mCurrentScene->mEntities[entityID].mCollider.mMaxRadius = r;
    mCurrentScene->mEntities[entityID].bCollidable = true;
    mCurrentScene->mEntities[entityID].mCollider.bIsDynamic = isDynamic;
}


bool ResourceManager::testCollision(Entity &a, Entity &b)
{

    //test sphere with sphere
    if (a.mCollider.mType == gsl::SPHERE && b.mCollider.mType == gsl::SPHERE)
    {
        gsl::vec3 Distance = b.mCollider.mMinCenter - a.mCollider.mMinCenter;
        float DistanceSqrd = gsl::vec3::dot(Distance, Distance);
        float rsSqrd = a.mCollider.mMaxRadius.length() + b.mCollider.mMaxRadius.length();

        return DistanceSqrd <= rsSqrd;
    }
    //box with box
    else if (a.mCollider.mType == gsl::OBB && b.mCollider.mType == gsl::OBB)
    {
        return a.mCollider.mMaxRadius >= b.mCollider.mMinCenter && b.mCollider.mMaxRadius >= a.mCollider.mMinCenter;
    }
    //box with sphere
    else if (a.mCollider.mType == gsl::OBB && b.mCollider.mType == gsl::SPHERE)
    {
        float r = b.mCollider.mMaxRadius.x;
        if (r < 0) r *= -1.f;

        gsl::vec3 halfBox = a.mCollider.mMaxRadius / 2.f;
        gsl::vec3 Distance = b.mCollider.mMinCenter - a.mCollider.mMinCenter;
        gsl::vec3 clamped;
        clamped.clamp(Distance, -halfBox, halfBox);
        gsl::vec3 cls = a.mCollider.mMinCenter + clamped;
        Distance = cls - b.mCollider.mMinCenter;

        return Distance.length() < r;
    }

    else return false;

}

void ResourceManager::updateColliders()
{
    for (MAX_ENTITIES_TYPE i{0}; i < mCurrentScene->mEntities.size(); i++)
    {
        if (mCurrentScene->mEntities[i].bCollidable)
        {
            if (mCurrentScene->mEntities[i].mCollider.bIsDynamic)
            {
                if (mCurrentScene->mEntities[i].mCollider.mType == gsl::OBB) setOBBCollider(i, mCurrentScene->mEntities[i].mTransform.position
                                                                                            - mCurrentScene->mEntities[i].mTransform.scale,
                                                                                            mCurrentScene->mEntities[i].mTransform.position
                                                                                            + mCurrentScene->mEntities[i].mTransform.scale,
                                                                                            mCurrentScene->mEntities[i].mCollider.bIsDynamic);
                else if (mCurrentScene->mEntities[i].mCollider.mType == gsl::SPHERE) setSphereCollider(i, 0, mCurrentScene->mEntities[i].mCollider.bIsDynamic);
            }
        }
    }
}

// -----------------------------------

bool ResourceManager::initSound()
{
    qDebug() << "Initialize OpenAL...";
    mDevice = alcOpenDevice(NULL);
    if (mDevice)
    {
        mContext = alcCreateContext(mDevice, NULL);
        alcMakeContextCurrent(mContext);
    }

    alGetError();

    if (!mDevice)
    {
        qDebug() << "Error: No sound device";
    }
    else
    {
        qDebug() << "Sound device made";
    }

    return true;
}

void ResourceManager::clnup()
{
    mContext = alcGetCurrentContext();
    mDevice = alcGetContextsDevice(mContext);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(mContext);
    alcCloseDevice(mDevice);
}

bool ResourceManager::checkError()
{
    switch (alGetError())
    {
    case AL_NO_ERROR:
        break;
    case AL_INVALID_NAME:
        qDebug() << "Invalid name!";
        return false;
    case AL_INVALID_ENUM:
        qDebug() << "Invalid enum!";
        return false;
    case AL_INVALID_VALUE:
        qDebug() << "Invalid value!";
        return false;
    case AL_INVALID_OPERATION:
        qDebug() << "Invalid operation!";
        return false;
    case AL_OUT_OF_MEMORY:
        qDebug() << "Out of memory!";
        return false;
    default: break;
    }
    return true;
}

void ResourceManager::setSound(std::string name, unsigned short EntID, std::string filePath, bool isLooping, float gain)
{
    mCurrentScene->mEntities[EntID].mSound.mName = name;
    mCurrentScene->mEntities[EntID].mSound.bIsLooping = isLooping;

    alGetError();
    alGenBuffers(1, &mCurrentScene->mEntities[EntID].mSound.mBuffer);
    checkWavError("alGenBuffers");
    alGenSources(1, &mCurrentScene->mEntities[EntID].mSound.mSrc);
    checkWavError("alGenSources");
    alSourcef(mCurrentScene->mEntities[EntID].mSound.mSrc, AL_PITCH, 1.0f);
    alSourcef(mCurrentScene->mEntities[EntID].mSound.mSrc, AL_GAIN, gain);

    ALfloat temp[3] = {mCurrentScene->mEntities[EntID].mTransform.position.x, mCurrentScene->mEntities[EntID].mTransform.position.y, mCurrentScene->mEntities[EntID].mTransform.position.z};
    alSourcefv(mCurrentScene->mEntities[EntID].mSound.mSrc, AL_POSITION, temp);
    ALfloat temp2[3] = {mCurrentScene->mEntities[EntID].mVelocity.x, mCurrentScene->mEntities[EntID].mVelocity.y, mCurrentScene->mEntities[EntID].mVelocity.z};
    alSourcefv(mCurrentScene->mEntities[EntID].mSound.mSrc, AL_VELOCITY, temp2);

    alSourcei(mCurrentScene->mEntities[EntID].mSound.mSrc, AL_LOOPING, mCurrentScene->mEntities[EntID].mSound.bIsLooping);

    if (filePath != "")
        loadWav(filePath, mCurrentScene->mEntities[EntID].mSound.mSrc, mCurrentScene->mEntities[EntID].mSound.mBuffer);
}

void ResourceManager::updateListener(gsl::vec3 pos, gsl::vec3 vel, gsl::vec3 dir, gsl::vec3 up)
{
    ALfloat posVector[3];
    ALfloat velVector[3];
    ALfloat headVector[6];
    posVector[0] = pos.x;
    posVector[1] = pos.y;
    posVector[2] = pos.z;
    velVector[0] = vel.x;
    velVector[1] = vel.y;
    velVector[2] = vel.z;
    headVector[0] = dir.x;
    headVector[1] = dir.y;
    headVector[2] = dir.z;
    headVector[3] = up.x;
    headVector[4] = up.y;
    headVector[5] = up.z;
    alListenerfv(AL_POSITION, posVector);
    alListenerfv(AL_VELOCITY, velVector);
    alListenerfv(AL_ORIENTATION, headVector);
}

bool ResourceManager::loadWav(std::string filePath, ALuint mSource, ALuint mBuffer)
{
    qDebug() << "Loading wave file!";
    ALuint frequency{};
    ALenum format{};
    wave_t* waveData = new wave_t();
    if (!WavHandler::loadWave(filePath, waveData))
    {
        qDebug() << "Error loading wave file!";
        return false; // error loading wave file data
    }

    frequency = waveData->sampleRate;

    switch (waveData->bitsPerSample)
    {
    case 8:
        switch (waveData->channels)
        {
        case 1: format = AL_FORMAT_MONO8;
            qDebug() << "Format: 8bit Mono";
            break;
        case 2: format = AL_FORMAT_STEREO8;
            qDebug() << "Format: 8bit Stereo";
            break;
        default: break;
        }
        break;
    case 16:
        switch (waveData->channels)
        {
        case 1: format = AL_FORMAT_MONO16;
            qDebug() << "Format: 16bit Mono";
            break;
        case 2: format = AL_FORMAT_STEREO16;
            qDebug() << "Format: 16bit Stereo";
            break;
        default: break;
        }
        break;
    default: break;
    }

    if (waveData->buffer == NULL)
    {
        qDebug() << " there is no buffer ";
    }

    std::ostringstream i2s;
    i2s << waveData->dataSize;
    std::string datasize;
    qDebug() << QString::fromStdString("DataSize: " + i2s.str() + " bytes");

    alGetError();
    alBufferData(mBuffer, format, waveData->buffer, waveData->dataSize, frequency);
    checkWavError("alBufferData");
    alSourcei(mSource, AL_BUFFER, mBuffer);
    checkWavError("alSourcei (loadWave)");

    qDebug() << "Loading complete!";
    if (waveData->buffer) delete waveData->buffer;
    if (waveData) delete waveData;
    return true;
}

bool ResourceManager::checkWavError(std::string name)
{
    switch (alGetError())
    {
    case AL_NO_ERROR:
        break;
    case AL_INVALID_NAME:
        qDebug() << QString::fromStdString("OpenAL Error: "+name+": Invalid name!");
        return false;
    case AL_INVALID_ENUM:
        qDebug() << QString::fromStdString("OpenAL Error: "+name+": Invalid enum!");
        return false;
    case AL_INVALID_VALUE:
        qDebug() << QString::fromStdString("OpenAL Error: "+name+": Invalid value!");
        return false;
    case AL_INVALID_OPERATION:
        qDebug() << QString::fromStdString("OpenAL Error: "+name+": Invalid operation!");
        return false;
    case AL_OUT_OF_MEMORY:
        qDebug() << QString::fromStdString("OpenAL Error: "+name+": Out of memory!");
        return false;
    default: break;
    }
    return true;
}

void ResourceManager::playSound(MAX_ENTITIES_TYPE EntID)
{
    alSourcePlay(mCurrentScene->mEntities[EntID].mSound.mSrc);
}
void ResourceManager::pauseSound(MAX_ENTITIES_TYPE EntID)
{
    alSourcePause(mCurrentScene->mEntities[EntID].mSound.mSrc);
}
void ResourceManager::stopSound(MAX_ENTITIES_TYPE EntID)
{
    alSourceStop(mCurrentScene->mEntities[EntID].mSound.mSrc);
}

void ResourceManager::startAllSound()
{
    for (MAX_ENTITIES_TYPE i{0}; i < mCurrentScene->mEntities.size(); i++)
    {
        alSourcePlay(mCurrentScene->mEntities[i].mSound.mSrc);
    }
}

void ResourceManager::stopAllSound()
{
    for (MAX_ENTITIES_TYPE i{0}; i < mCurrentScene->mEntities.size(); i++)
    {
        alSourceStop(mCurrentScene->mEntities[i].mSound.mSrc);
    }
}

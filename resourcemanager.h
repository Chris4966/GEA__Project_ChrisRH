#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>

#include <map>
#include <vector>
#include <string>
#include <qdebug.h>
#include "gltypes.h"
#include "texture.h"
#include "ecs/components.h"
#include "ecs/entity.h"
#include "vertex.h"
#include "Sound/wavhandler.h"
#include "gslpch.h"

#ifdef _WIN32
#include "AL/al.h"
#include "AL/alc.h"
#endif
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif



/**
    @brief Resource Manager Class.
 */
class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:
    bool searchMap(std::string &name, GLuint &index);

    static ResourceManager& getInstance();

    std::vector <class MeshObject*> mMeshObjects;   /// holds all different mesh objects - reused when needed
    std::map<std::string, unsigned int> mMeshObjectMap;      /// name / index to mMeshObjects

    std::map<std::string, unsigned short> mMaterialNames;  //name / MaterialID
    std::map<std::string, int> mTextureNames;   //name / TextureID
    std::map<unsigned short, char> mShaderProgramIndex;     //shader program, index into mShaderProgram

    int mTextureSlots[32];      //what textureID in what slot

    void readTextures();
    void readObjs();
    void readShaders(class RenderWindow* renderWindow);
    void makeMaterials();

    class Texture *mTexture[gsl::NumberOfTextures]{nullptr};
    struct ecs::Material *mMaterial[gsl::NumberOfMaterials]{nullptr};
    class Shader *mShaderProgram[gsl::NumberOfShaders]{nullptr};

    /** Creates an entity.
       @param meshName - Name of the mesh.
       @param transformIn - Transforms: Position, Scale, Rotation.
       @param materialName - Name of the Material.
       @return - Returns entity ID.
     */
    MAX_ENTITIES_TYPE make3DEntity(std::string meshName, ecs::Transform transformIn, std::string materialName);
    /** Creates an entity that emits light.
       @param meshName - Name of the mesh.
       @param transformIn - Transforms: Position, Scale, Rotation.
       @param materialName - Name of the Material.
       @param lightIn - Light component.
       @return - Returns entity ID.
     */
    MAX_ENTITIES_TYPE makeLightEntity(std::string meshName, ecs::Transform transformIn, std::string materialName, ecs::Light *lightIn);

    /** Sets a mesh on an existing entity.
       @param entityID
       @param meshName
     */
    void setMesh(MAX_ENTITIES_TYPE entityID, std::string meshName);

    /** Sets transforms on an existing entity.
       @param entityID
       @param TMatrix - Transform matrix.
     */
    void setTransform(MAX_ENTITIES_TYPE entityID, gsl::mat4 TMatrix);

    /** Sets material on an existing entity.
       @param entityID
       @param matName - Material Name.
     */
    void setMaterial(MAX_ENTITIES_TYPE entityID, std::string matName);

    /** Sets script on an existing entity.
       @param entityID
       @param fileName
     */
    void setScript(MAX_ENTITIES_TYPE entityID, std::string fileName);

    /** Sets a string tag on an existing entity.
       @param entityID
       @param tagName
     */
    void setTag(MAX_ENTITIES_TYPE entityID, std::string tagName);

    /** Sets sound on an existing entity.
       @param name - Gives a name to the sound.
       @param EntID - Entity ID.
       @param filePath - Where the sound file is located.
       @param isLooping - Whether or not to loop the sound.
       @param gain - Volume of the sound.
     */
    void setSound(std::string name, MAX_ENTITIES_TYPE EntID, std::string filePath= "", bool isLooping = false, float gain = 1.f);

    /** Sets a bounding box collider on an existing entity.
        @param entityID
        @param min - Minimum position for bounding box.
        @param max - Maximum position for bounding box.
        @param isDynamic - Whether or not the object will move.
     */
    void setOBBCollider(MAX_ENTITIES_TYPE entityID, gsl::vec3 min = {0, 0, 0}, gsl::vec3 max = {0, 0, 0}, bool isDynamic = false);

    /** Sets a spherical collider on an existing entity.
       @param entityID
       @param r - Radius of the sphere.
       @param isDynamic - Whether or not the object will move.
     */
    void setSphereCollider(MAX_ENTITIES_TYPE entityID, float r = 0, bool isDynamic = false);

    /** Checks if two entities are colliding.
       @param a - First Entity.
       @param b - Second Entity.
       @return - Returns true if collided, false if not.
     */
    bool testCollision(Entity &a, Entity &b);

    /** Updates position for dynamic colliders.
     */
    void updateColliders();

    int mIndexOfLastTexture{-1};

    class CoreEngine *mCoreEngine{nullptr};
    class Scene *mCurrentScene{nullptr};

    struct SoundListener
    {
        gsl::vec3 pos, vel, dir, up;
        SoundListener()
        {
            pos = {0.f, 0.f, 0.f};
            vel = {0.f, 0.f, 0.f};
            dir = {0.f, 0.f, 1.f};
            up  = {0.f, 1.f, 0.f};
        }
    };

    //sound stuff

    /** Initializes Sound System.
       @return - Returns true if sound is used.
     */
    bool initSound();

    /** Cleans up sound.
     */
    void clnup();

    /** Updates listener position.
       @param pos - position.
       @param vel - velocity.
       @param dir - moving direction.
       @param up - upwards direction.
     */
    void updateListener(gsl::vec3 pos, gsl::vec3 vel, gsl::vec3 dir, gsl::vec3 up);

    /** Loads Wav file.
       @param filePath - location of wav file.
       @param src - sound source.
       @param buffer - sound buffer.
       @return
     */
    bool loadWav(std::string filePath, ALuint src, ALuint buffer);

    bool checkWavError(std::string name);
    void playSound(MAX_ENTITIES_TYPE EntID);
    void pauseSound(MAX_ENTITIES_TYPE EntID);
    void stopSound(MAX_ENTITIES_TYPE EntID);
    void startAllSound();
    void stopAllSound();

private:
    ResourceManager();
    void operator=(ResourceManager&);           //Assignment operator
    ResourceManager(const ResourceManager&);    //Copy constructor

    bool checkError();

    ALCdevice *mDevice;
    ALCcontext *mContext;

    SoundListener mListener;

};

#endif // RESOURCEMANAGER_H

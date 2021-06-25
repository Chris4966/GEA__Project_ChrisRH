#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "gslpch.h"
#include "gltypes.h"
#include "vertex.h"

#ifdef _WIN32
#include "AL/al.h"
#include "AL/alc.h"
#endif
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

/**
    @brief Entity Components.
 */

namespace ecs {

struct Transform
{
    gsl::vec3 position{0, 0, 0};
    gsl::vec3 rotation{0, 0, 0};
    gsl::vec3 scale{1};
};

struct Collider
{
    gsl::Colliders mType{gsl::NOCOLLIDER};
    gsl::vec3 mMinCenter{-.5f, -.5f, -.5f};     //xyzMin, xyzMax if box, center, radius if sphere
    gsl::vec3 mMaxRadius{.5f, .5f, .5f};
    bool bIsDynamic{false}; //static or dynamic collider
};

struct MaterialID
{
    MAX_ENTITIES_TYPE mEntityID{MAX_ENTITIES};
    unsigned short mMaterialIndex{0};        //Material 0 = default material - should always exist!
};

struct Face
{
    Face(Vertex v1, Vertex v2, Vertex v3) : V1{v1}, V2{v2}, V3{v3}
    {

    }

    Vertex V1, V2, V3;
};

struct Mesh
{
    unsigned int mVAO{65535};
    unsigned short mVertexCount{65535};
    unsigned short mIndexCount{65535};
    std::vector<Face> mFaces;

    //animation rigging stuff if implemented
    std::vector<int> mJointIDs;
    std::vector<float> mWeights;
};

struct Sound
{
    std::string mName;
    ALuint mSrc, mBuffer;
    bool bIsLooping{false};
    float mGain{1.f};
};

struct Material
{
    unsigned short mMaterialID{65535};
    unsigned short mShader{gsl::NOSHADER};
    unsigned int mSpecularExponent{0};
    unsigned short mTextureId{0};
    GLfloat mSpecularStrenght{0.f};
    gsl::vec3 mColor{};
    std::string mMatName;
};

struct Light
{
    gsl::LightTypes mType{gsl::POINT};

    GLfloat mAmbientStrength{0.5f};
    gsl::vec3 mAmbientColour{0.3f, 0.3f, 0.3f};
    GLfloat mLightStrength{0.7f};
    gsl::vec3 mLightColour{0.9f, 0.9f, 0.9f};
    GLfloat mSpecularStrength{0.3f};
    gsl::vec3 mSpecularColour{0.9f, 0.9f, 0.9f};
    GLint mSpecularExponent{4};

    gsl::vec3 mDirection{0.f, -1.f, 0.f};

    GLfloat mLineAttenuation{0.22f}, mQuadAttenuation{0.2f};
    GLfloat mInnerSpotAngle{20.f};
    GLfloat mOuterSpotAngle{25.f};
};

struct Script
{
    std::string mName;

    //data for player
    QString pName;
    int pLevel;
    double pHP, pStr, pSpeed;
};



}   //namespace

#endif // COMPONENTS_H

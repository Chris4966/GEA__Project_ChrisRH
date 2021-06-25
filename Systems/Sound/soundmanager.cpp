#include "soundmanager.h"
#include "wavhandler.h"
#include <iostream>

SoundManager* SoundManager::mInstance = NULL;

SoundManager::SoundManager() : mDevice(NULL), mContext(NULL)
{

}

bool SoundManager::init()
{
    std::cout << "Initilize OpenAL!\n";
    mDevice = alcOpenDevice(NULL);
    if (mDevice)
    {
        mContext = alcCreateContext(mDevice, NULL);
        alcMakeContextCurrent(mContext);
    }

    // Generate buffers
    alGetError();

    if (!mDevice)
    {
        std::cout << "No device \n";
    }
    else
        std::cout << "Device made \n";


    return true;
}

void SoundManager::cleanUp()
{
    mContext = alcGetCurrentContext();
    mDevice = alcGetContextsDevice(mContext);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(mContext);
    alcCloseDevice(mDevice);
}

bool SoundManager::checkError()
{
    switch (alGetError())
    {
    case AL_NO_ERROR:
        break;
    case AL_INVALID_NAME:
        std::cout << "Invalid name!\n";
        return false;
    case AL_INVALID_ENUM:
        std::cout << "Invalid enum!\n";
        return false;
    case AL_INVALID_VALUE:
        std::cout << "Invalid value!\n";
        return false;
    case AL_INVALID_OPERATION:
        std::cout << "Invalid operation!\n";
        return false;
    case AL_OUT_OF_MEMORY:
        std::cout << "Out of memory!\n";
        return false;
    default: break;
    }
    return true;
}

SoundSrc* SoundManager::makeSource(std::string name, gsl::vec3 pos, std::string fileName, bool loop, float soundLevel)
{
    SoundSrc* tempPtr = new SoundSrc(name, loop, soundLevel);
    tempPtr->setPos(pos);
    if (fileName != "")
        tempPtr->loadWave(fileName);
    return tempPtr;
}

void SoundManager::updateListener(gsl::vec3 pos, gsl::vec3 vel, gsl::vec3 dir, gsl::vec3 up)
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

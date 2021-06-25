#ifndef SOUNDSRC_H
#define SOUNDSRC_H

#ifdef _WIN32
#include "include/AL/al.h"
#include "include/AL/alc.h"
#endif
#ifdef __APPLE__
#include <include/AL/al.h>
#include <include/AL/alc.h>
#endif

#include <string>
#include "gslpch.h"

/**
    @brief Sound Source class to use with soundmanager
 */

class SoundSrc
{
public:
    SoundSrc(std::string name, bool loop = false, float gain = 1.0);
    ~SoundSrc();

    void play();
    void pause();
    void stop();

    //loads a given Wav file
    bool loadWave(std::string filePath);

    void setPos(gsl::vec3 newPos);               // sets position of a source
    gsl::vec3 getPos() {return mPos;}            // gets position of a source
    void setVelocity(gsl::vec3 newVel);          // sets velocity of a source
    gsl::vec3 getVelocity() {return mVel;}       // gets velocity of a source

private:
    std::string mName;          // name of sorce
    ALuint mSource;             // sound sorce
    ALuint mBuffer;             // sound buffer
    gsl::vec3 mPos;              // position of source
    gsl::vec3 mVel;              // velocity of source
    bool checkError(std::string name);
};

#endif // SOUNDSRC_H

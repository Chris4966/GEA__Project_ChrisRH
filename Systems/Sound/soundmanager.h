#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#ifdef _WIN32
#include "include/AL/al.h"
#include "include/AL/alc.h"
#endif
#ifdef __APPLE__
#include <includeAL/al.h>
#include <includeAL/alc.h>
#endif

#include <string>
#include <vector>
#include "soundsrc.h"
#include "gslpch.h"

/**
    @brief System class for managing sound using OpenAL.
 */

class SoundManager
{
public:
    struct SoundListener
    {
        gsl::vec3 pos;
        gsl::vec3 vel;
        gsl::vec3 dir;
        gsl::vec3 up;
        SoundListener()
        {
            pos = gsl::vec3(0.0f, 0.0f, 0.0f);
            vel = gsl::vec3(0.0f, 0.0f, 0.0f);
            dir = gsl::vec3(0.0f, 0.0f, 1.0f);
            up  = gsl::vec3(0.0f, 1.0f, 0.0f);

        }
    };

    static SoundManager* getInstance()          //get pointer to instance
    {
        if (!mInstance)
            mInstance = new SoundManager();
        return mInstance;
    }

    bool init();
    void cleanUp();

    /** Creates a Sound Source.
       @param name - sound name.
       @param pos - position.
       @param filePath - sound file location.
       @param loop - is it looping.
       @param gain - volume.
       @return - returns a source pointer.
     */
    SoundSrc* makeSource(std::string name, gsl::vec3 pos, std::string filePath = "", bool loop = false, float gain = 1.0);

    /** Updates Sound Listener.
       @param pos - position.
       @param vel - velocity.
       @param dir - direction.
       @param up - up vector.
     */
    void updateListener(gsl::vec3 pos, gsl::vec3 vel, gsl::vec3 dir, gsl::vec3 up);

private:
    SoundManager();
    SoundManager(SoundManager const&);
    void operator=(SoundManager const&);
    static SoundManager* mInstance;

    bool checkError();

    ALCdevice* mDevice;
    ALCcontext* mContext;

    SoundListener mListener;
};

#endif

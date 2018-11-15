#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

// std includes
#include <map>
#include <string>

// external libs includes
#include <AL/al.h>
#include <AL/alc.h>

// project includes
#include "resources/singleton.h"

class AudioManager: public Singleton<AudioManager>
{
public:
    void startUp();
    void shutDown();

    void createSource(const std::string& name, const std::string& path, bool isLooping);
    void playSource(const std::string& name);
    void stopSource(const std::string& name);
    void pauseSource(const std::string& name);
private:
    ALCdevice* device_;
    ALCcontext* context_;
    
    std::map<std::string, std::pair<ALuint, ALuint>> sources_;
};

#endif
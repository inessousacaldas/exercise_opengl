#ifndef ASSETS_LOADER_H
#define ASSETS_LOADER_H

// std includes
#include <string>

class AssetsLoader 
{
public:
    static void loadShaders();
    static void loadTextures();
    static void loadAudioFiles();

    static const std::string RESOURCES_FOLDER;
};

#endif
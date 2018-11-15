#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

// std includes
#include <vector>

// external libs includes
#include <GL/glew.h>
#include <string>

// project includes
#include "audio/audio_file.h"

class FileManager
{
public:
    static std::string readAsText(const std::string& path);
    static std::vector<unsigned char> readAsBinary(const std::string& path);
    static unsigned char* readImage(const std::string& path,
                                    GLint width,
                                    GLint height,
                                    GLint components,
                                    bool flip = false);
    static AudioFile readOggFile(const std::string& path);
};


#endif
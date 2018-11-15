#ifndef FONT_H
#define FONT_H

// std includes
#include <vector>

// external libs includes
#include "stb_truetype.h"

using FontText = struct 
{
    float size = 40.0f;
    const int atlasWidth = 2048;
    const int atlasHeight = 2048;
    const int overSampleX = 4;
    const int overSampleY = 4;
    const char firstChar = 0;
    const int charCount = 127;
    std::vector<stbtt_packedchar> charInfo;
};

#endif
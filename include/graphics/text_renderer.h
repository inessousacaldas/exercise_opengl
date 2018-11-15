#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

// std includes
#include <string>
#include <map>

// external libs includes
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// project includes
#include "graphics/shader.h"
#include "graphics/font.h"
#include "graphics/glyph_info.h"

#include "resources/singleton.h"

class TextRenderer: public Singleton<TextRenderer> 
{
public:
    void init(const std::string& path, Shader* shader);
    void renderText(const std::string& text, 
                           const glm::vec2& position, 
                           const glm::vec3& color, 
                           float scale = 1.0f);
private:
    Shader* shader_;
    GLuint VAO_;
    GLuint VBO_;
    GLuint textureId_;

    FontText font_;

    std::map<char, GlyphInfo> glyphs_;
    
    void loadFont(const std::string& path);
    void initGlyphs();
    void initVAO();

    GlyphInfo getGlyphInfo(char character, float offsetX, float offsetY);

    TextRenderer();
    ~TextRenderer();

friend Singleton;
};

#endif

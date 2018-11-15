#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

// external libs includes
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// project includes
#include "resources/singleton.h"

#include "graphics/shader.h"
#include "graphics/texture.h"

class SpriteRenderer: public Singleton<SpriteRenderer> 
{
public:
    void init(Shader* shader);
    void renderSprite(const Texture* texture,
                      const glm::vec2& position,
                      const glm::vec2& size = glm::vec2(10, 10),
                      const glm::vec3& color = glm::vec3(1.0f),
                      GLfloat rotate = 0.0f) const;

    // setters
    void shader(Shader* shader) { shader_ = shader; };

private:
    Shader* shader_;
    GLuint VAO_;
    GLuint VBO_;

    SpriteRenderer();
    ~SpriteRenderer();

friend Singleton;
};

#endif
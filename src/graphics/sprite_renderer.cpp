#include "graphics/sprite_renderer.h"

// external libs includes
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer() {}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
}

void SpriteRenderer::init(Shader* shader) 
{
    this->shader(shader);

    GLfloat vertices[] = 
    {
        // Pos      // Tex
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::renderSprite(const Texture* texture,
                                  const glm::vec2& position,
                                  const glm::vec2& size,
                                  const glm::vec3& color,
                                  GLfloat rotate) const 
{
    shader_->use();

    glm :: mat4 model = glm :: mat4 (1.0);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader_->setUniform("model", model);
    shader_->setUniform("color", color);

    texture->bind();

    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    glEnable(GL_TEXTURE_2D);
    shader_->end();
    texture->unbind();
}

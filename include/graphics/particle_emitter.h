#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

// std includes
#include <vector>

// external libs includes
#include <GL/glew.h>
#include <glm/vec2.hpp>

// project includes
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "graphics/particle.h"

#include "game/ball.h"

class ParticleEmitter
{
public:
    ParticleEmitter(Shader* shader, Texture* texture, GLuint amount);
    ~ParticleEmitter();

    void update(float delta,
                const Ball& object,
                int newParticles,
                const glm::vec2 offset = glm::vec2(0.0f));
    void render(float scale);

    void reset() {particles_.clear(); };

private:
    std::vector<Particle> particles_;
    GLuint amount_;
    Shader* shader_;
    Texture* texture_;

    GLuint VAO_;
    GLuint VBO_;

    GLuint lastUsedParticle_;

    GLuint getFirstUnusedParticle();
    void respawnParticle(Particle& particle,
                         const Ball& object,
                         const glm::vec2& offset = glm::vec2(0.0f));
};

#endif
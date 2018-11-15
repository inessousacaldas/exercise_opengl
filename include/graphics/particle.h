#ifndef PARTICLE_H
#define PARTICLE_H

// external libs includes
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct Particle
{
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 color;

    float lifespan;

    Particle() : position(0.0f), velocity(0.0f), color(1.0f), lifespan(0.0f) {}
};

#endif

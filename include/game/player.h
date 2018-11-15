#ifndef PLAYER_H
#define PLAYER_H

// external libs includes
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "game/game_object.h"

class Player: public GameObject 
{
public:
    Player(const glm::vec2& position,
           const glm::vec2& size,
           const glm::vec3& color,
           Texture* sprite,
           float velocity,
           const glm::vec2& boundaries);
    ~Player();

    // getters
    float velocity() const { return velocity_; };
    const glm::vec2& boundaries() const { return boundaries_; };

    // setters
    void boundaries(const glm::vec2& boundaries) { boundaries_ = boundaries; }

private:
    float velocity_;
    glm::vec2 boundaries_;
};

#endif
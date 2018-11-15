#ifndef POWER_UP_H
#define POWER_UP_H

// external libs includes
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// project includes
#include "game/game_object.h"
#include "game/ball.h"
#include "game/player.h"
#include "game/power_up_type.h"

#include "graphics/post_processor.h"


class PowerUp: public GameObject 
{
public:
    PowerUp(const glm::vec2& position,
            const glm::vec2& size,
            const glm::vec3& color,
            Texture* sprite,
            const glm::vec2& velocity,
            PowerUpType type,
            GLfloat duration);
    ~PowerUp();

    void update(float delta, Player& player, Ball& ball, PostProcessor& postProcessor);
    void activate(Player& player, Ball& ball, PostProcessor& postProcessor, const glm::vec2& scales);

    // getters
    PowerUpType type() const { return type_; }
    GLfloat duration() const { return duration_; }
    bool isActivated() const { return isActivated_; }
    bool isDestroyed() const { return isDestroyed_; }
    const glm::vec2& velocity() const { return velocity_; }

    // setters
    void isDestroyed(bool isDestroyed) { isDestroyed_ = isDestroyed; }

private:
    PowerUpType type_;
    GLfloat duration_;
    bool isActivated_;
    bool isDestroyed_;
    glm::vec2 velocity_;
    float sizeUpdate_;
};

#endif
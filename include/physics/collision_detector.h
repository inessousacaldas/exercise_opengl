#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

// external libs includes
#include <glm/vec2.hpp>

// project includes
#include "physics/collision.h"
#include "physics/direction.h"

#include "game/game_object.h"
#include "game/ball.h"

class CollisionDetector 
{
public:
    static Collision checkCollision(const Ball& ball, const GameObject& gameObject);
    static bool checkCollision(const GameObject& one, const GameObject& two);
private:
    static Direction getVectorDirection(const glm::vec2& target);
};

#endif

#ifndef COLLISION_H
#define COLLISION_H

// std includes
#include <tuple>

// external libs includes
#include <glm/vec2.hpp>

// project includes
#include "physics/direction.h"

using Collision = std::tuple<bool, Direction, glm::vec2>;

#endif
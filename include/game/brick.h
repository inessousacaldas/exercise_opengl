#ifndef BRICK_H
#define BRICK_H

// std includes
#include <map>
#include <memory>

// external libs includes
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// project includes
#include "game/game_object.h"
#include "game/animation.h"

class Brick: public GameObject 
{
public:
    Brick(const glm::vec2& position,
          const glm::vec2& size,
          const glm::vec3& color,
          Texture* sprite,
          bool isSolid = false);
    ~Brick();

    
    void startAnimation(GLfloat delay); // animates the brick
    bool isAnimating(); // checks if any animation is active
    
    // getters
    bool isSolid() const { return isSolid_; }
    bool isDestroyed() const { return isDestroyed_; }
    bool isPrepared() const { return animations_.size() == 0 ? true : false ; }

    // setters
    void isDestroyed(bool isDestroyed) { isDestroyed_ = isDestroyed; }

    void render (const SpriteRenderer& renderer)  override;

private:

    bool isSolid_;
    bool isDestroyed_;

    GLfloat lastTime_; // last call for rendering
    GLfloat deltaTime_;

    std::map<std::string, std::unique_ptr<Animation>> animations_;

    glm::vec2 scale_;

};

#endif
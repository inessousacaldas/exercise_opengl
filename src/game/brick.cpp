#include "game/brick.h"

// external libs includes
#include "GLFW/glfw3.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/color_space.hpp>

// TODO 
#include <iostream>
#include <cmath>
// project includes
#include "game/animation_type.h"

Brick::Brick(const glm::vec2& position,
             const glm::vec2& size,
             const glm::vec3& color,
             Texture* sprite,
             bool isSolid)
: GameObject(position, size, color, sprite), isSolid_(isSolid), isDestroyed_(false), scale_(glm::vec2(1.0f)) 
{

    // Add scale animation
    glm::vec3 initialScale = glm::vec3(0.0);
    glm::vec3 finalScale = glm::vec3(1.0);
    GLfloat animationTime = glfwGetTime();
    std::string name = std::to_string(animationTime);

    animations_["scale"] = std::unique_ptr<Animation>( new Animation(
                                                    AnimationType::SCALE, 
                                                    initialScale, 
                                                    finalScale, 
                                                    0.0, 
                                                    animationTime));

    // Add color animation
    glm::vec3 initialColor = glm::vec3(1.0);
    glm::vec3 finalColor = color_;
    animationTime = glfwGetTime();

    name = std::to_string(animationTime);

    animations_["color"] = std::unique_ptr<Animation>( new Animation(
                                                     AnimationType::COLOR, 
                                                    initialColor, 
                                                    finalColor, 
                                                    0.0, 
                                                    animationTime));

    }

Brick::~Brick() {}


bool Brick::isAnimating() 
{
    for (auto& animation: animations_)
    {
        if (animation.second->active) return true;
    }

    return false;
}

void Brick::startAnimation(GLfloat delay)
{
    // Color animation
    animations_["scale"].get()->active = true;
    animations_["scale"].get()->animationStartTime = glfwGetTime();
    animations_["scale"].get()->initialState = glm::vec3(0.0);
    animations_["scale"].get()->currentState = glm::vec3(0.0);
    animations_["scale"].get()->delay = delay;

    scale_ = animations_["scale"].get()->initialState;


    glm::vec3 initialColor = glm::vec3(fabs(1.0 - color_.x), fabs(1.0 - color_.y), fabs(1.0 - color_.z));

    animations_["color"].get()->active = true;
    animations_["color"].get()->animationStartTime = glfwGetTime();
    animations_["color"].get()->initialState = initialColor;
    animations_["color"].get()->currentState = initialColor;
    animations_["color"].get()->delay = delay;

    color_ = animations_["color"].get()->initialState;
    
}

void Brick::render(const SpriteRenderer& renderer)
{
    glm::vec2 position = position_;   
    glm::vec2 size;
    glm::vec3 colorAmount;
    
    GLfloat timer;
    // Time variables
    GLfloat now = glfwGetTime();
    deltaTime_ = (now - lastTime_);
    lastTime_ = glfwGetTime();

    for(auto& animation : animations_)
    {

        // If time elapsed greater than delay
        if((animation.second->animationStartTime + animation.second->delay) < now && animation.second->active)
        {
            switch(animation.second->animationType)
            {
                case AnimationType::SCALE:

                    float scaleAmountX;
                    float scaleAmountY;

                    timer = now - (animation.second->animationStartTime + animation.second->delay);

                    scaleAmountX = animation.second->initialState.x +  timer * (animation.second->finalState.x  - animation.second->initialState.x );
                    scaleAmountY = animation.second->initialState.y +  timer * (animation.second->finalState.y  - animation.second->initialState.y );
                    
                    // Scale
                    scale_ = glm::vec2(scaleAmountX, scaleAmountY);

                    // Update animation current state
                    // if difference within resolution, is considered done
                    if(fabs(animation.second->finalState.x - animation.second->currentState.x) < animation.second->resolution &&
                        fabs(animation.second->finalState.y - animation.second->currentState.y) < animation.second->resolution)
                    {
                        animation.second->currentState = animation.second->finalState;
                        
                        scale_ = glm::vec2(animation.second->finalState.x, animation.second->finalState.y);
                    }
                    else
                    {
                        animation.second->currentState = glm::vec3(scale_.x, scale_.y, animation.second->finalState.z);
                    }

                    size = scale_*size_;

                    // Translate so the brick maintains position
                    position = position_ + glm::vec2((size_ - size).x/2, (size_ - size).y/2);

                break;

                case AnimationType::COLOR:

                    glm::vec3 color2  = animation.second->finalState;
                    glm::vec3 color1  = animation.second->initialState;
                    timer = now - (animation.second->animationStartTime + animation.second->delay);
                    glm::vec3 colorAmount = (color2 - color1) * timer + color1;
                    
                    color_ = colorAmount;

                    if(fabs(animation.second->currentState.x - animation.second->finalState.x) < animation.second->resolution &&
                        fabs(animation.second->currentState.y - animation.second->finalState.y) < animation.second->resolution &&
                        fabs(animation.second->currentState.z - animation.second->finalState.z) < animation.second->resolution)
                        
                    {
                        animation.second->currentState = animation.second->finalState;
                        color_ = animation.second->finalState;
                    }
                    else
                    {
                        animation.second->currentState = colorAmount;
                    }

                    

                break;
            }
        }
        
        if(animation.second->currentState == animation.second->finalState)
        {
            animation.second->active = false;
        }
    }

    renderer.renderSprite(sprite_, position, scale_*size_, color_);
}
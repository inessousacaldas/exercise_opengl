#ifndef ANIMATION_H
#define ANIMATION_H

// external libs includes
#include <glm/vec3.hpp>

// project includes
#include "game/animation_type.h"

struct Animation
{

    AnimationType animationType;
    glm::vec3 initialState;
    glm::vec3 finalState;
    glm::vec3 currentState;

    GLfloat delay; // waits some time before starting animation 
    GLfloat animationStartTime;
    GLfloat resolution;

    bool active;
    
    Animation() {};
    Animation(AnimationType aType, 
            glm::vec3 iState, 
            glm::vec3 fState, 
            GLfloat delayTime,
            GLfloat animationStart)
     : animationType(aType), initialState(iState), finalState(fState), currentState(iState),
     delay(delayTime), animationStartTime(animationStart), resolution(0.01), active(false){}

};


#endif
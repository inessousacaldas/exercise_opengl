#include "game/button.h"
// TODO
#include <iostream>

Button::Button(const glm::vec2& position,
            const glm::vec2& size,
            const glm::vec3& color,
            Texture* sprite, 
            Texture* spriteFocus,
            Texture* spritePressed, 
            std::function<void()> const &callback)
: GameObject(position, size, color, sprite), callback_(callback), state_ (ButtonState::NORMAL)
{
    textures_[ButtonState::NORMAL] = std::unique_ptr<Texture>(sprite);
    textures_[ButtonState::HIGHLIGHTED] = std::unique_ptr<Texture>(spriteFocus);
    textures_[ButtonState::PRESSED] = std::unique_ptr<Texture>(spritePressed);
}


Button::~Button() {}

void Button::destroy () 
{
    for(auto& texture : textures_)
    {
        texture.second->destroy();
    }
}

bool Button::hoverButton(glm::vec2 position, InputMouse action)
{
    bool hoverButton = false;

    if ( (position.x >= position_.x && position.x <= (position_.x + size_.x)) &&
        (position.y >= position_.y && position.y <= (position_.y + size_.y) ))
    {
        hoverButton = true;
    }
   
    if(state_ == ButtonState::PRESSED)
    {
        if(action == InputMouse::RELEASE)
        {
            state_ = (hoverButton ? ButtonState::HIGHLIGHTED : ButtonState::NORMAL);
            OnButtonReleased();
        }
                
    }
    else 
    {
         if(action == InputMouse::PRESS && hoverButton)
         {
            state_ = ButtonState::PRESSED;
         }
               
        else
        {
            state_ = (hoverButton ? ButtonState::HIGHLIGHTED : ButtonState::NORMAL);
        }
    }
    
    return false;
}

void Button::OnButtonReleased()
{
    callback_();
}

void Button::render (const SpriteRenderer& renderer) 
{
    renderer.renderSprite(textures_[state_].get(), position_, size_, color_);
}


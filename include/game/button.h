#ifndef BUTTON_H
#define BUTTON_H

// std includes
#include <memory>
#include <map>
#include <string>
#include <functional>

// external libs includes
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// project includes
#include "game/game_object.h"
#include "game/game_level.h"
#include "game/button_state.h"
#include "graphics/texture.h"
#include "input/input_mouse.h"

class Button : public GameObject
{
public:
    Button(const glm::vec2& position,
          const glm::vec2& size,
          const glm::vec3& color,
          Texture* sprite, Texture* spriteFocus, Texture* spritePressed, std::function<void()> const &callback);

    ~Button();
    
    void destroy();
    bool hoverButton(glm::vec2 position, InputMouse action);
    
    void render (const SpriteRenderer& renderer)  override;


private:
    std::map<ButtonState, std::unique_ptr<Texture>> textures_;
    ButtonState state_;
    std::function<void()> callback_;

    void OnButtonReleased(); // call the function callback
};

#endif
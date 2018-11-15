#ifndef MENU_H
#define MENU_H

// std includes
#include <memory>
#include <map>
#include <string>
#include <functional>

// external libs includes
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// project includes
#include "game/game_object.h"
#include "game/button.h"
#include "input/input_mouse.h"


class Menu: public GameObject 
{
public:
    Menu(const glm::vec2& position,
          const glm::vec2& size,
          const glm::vec3& color,
          Texture* sprite);
    ~Menu();

    void addButton(const std::string& name, 
                    const glm::vec2 &position,
                    const glm::vec2 &size,
                    const glm::vec3 &color,
                    Texture* sprite, 
                    Texture* spriteFocus, 
                    Texture* spritePressed, std::function<void()> const &calback);

    void destroy();
    
    void addCreditsFrame( const glm::vec2 &position,
                    const glm::vec2 &size,
                    const glm::vec3 &color,
                    Texture* sprite);
    
    void hoverButtons(glm::vec2 position, InputMouse action);

    void render (const SpriteRenderer& renderer)  override;

private:
    std::map<std::string, std::unique_ptr<Button>> buttons_;
    GameObject* creditsFrame_;
};


#endif
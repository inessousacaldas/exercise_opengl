#include "game/menu.h"

Menu::Menu(const glm::vec2 &position,
               const glm::vec2 &size,
               const glm::vec3 &color,
               Texture* sprite)
: GameObject(position, size, color, sprite) {}

Menu::~Menu() {}

void Menu::destroy()
{
    for(auto& button : buttons_)
    {
        button.second->destroy();
    }
}

void Menu::addButton(const std::string& name, 
                    const glm::vec2 &position,
                    const glm::vec2 &size,
                    const glm::vec3 &color,
                    Texture* sprite, 
                    Texture* spriteFocus, 
                    Texture* spritePressed, 
                    std::function<void()> const &calback)
{
    buttons_[name] = std::unique_ptr<Button>( new Button(position, size, color, sprite, spriteFocus, spritePressed, calback));
}

void Menu::addCreditsFrame( const glm::vec2 &position,
                    const glm::vec2 &size,
                    const glm::vec3 &color,
                    Texture* sprite)
{
    creditsFrame_ = new GameObject(position, size, color, sprite);               
}

 void Menu::hoverButtons(glm::vec2 position, InputMouse action)
 {
     for(auto &button : buttons_) 
    {
        button.second->hoverButton(position, action);
    }
 }

void Menu::render (const SpriteRenderer& renderer) 
{
    GameObject::render(renderer); // calls base class' function
    
    // Render buttons
    for(auto &button : buttons_) 
    {
        button.second->render(renderer);
    }

    // Render credits frame (on top of buttons)
    creditsFrame_->render(renderer);
}

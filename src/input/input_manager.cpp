#include "input/input_manager.h"

// external libs includes
#include <GLFW/glfw3.h>

// project includes
#include "input/event_handlers.h"

void InputManager::startUp()
{
    coordinates_ = glm::vec2(0.0);
}

void InputManager::shutDown() 
{
    keyHandlers_.clear();
}

void InputManager::pollEvents(float delta)
{
    glfwPollEvents();

    for (const auto& keyHandler : keyHandlers_)
    {
        keyHandler.second(delta);
    }

    mouseHandler();
}

void InputManager::processKeyEvent(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) 
    {
        keys_[key] = true;
    } 
    else if (action == GLFW_RELEASE)
    {
        keys_[key] = false;
        processedKeys_[key] = false;
    }
}

void InputManager::addKeyHandler(const std::string& name, KeyHandler keyHandler)
{
    keyHandlers_[name] = keyHandler;
}

void InputManager::removeKeyHandler(const std::string& name)
{
    keyHandlers_.erase(name);
}

void InputManager::mouseHandler()
{
    glfwSetWindowUserPointer(window_, this);

    auto funcPress = [](GLFWwindow* w, int button, int action , int mods)
    {
        static_cast<InputManager*>(glfwGetWindowUserPointer(w))->mouseButtonPressed(w, button, action, mods);
    };

    glfwSetMouseButtonCallback(window_, funcPress);

    auto funcCoords = [](GLFWwindow* w, double xPos, double yPos)
    {
        static_cast<InputManager*>(glfwGetWindowUserPointer(w))->updateMouseCoords(w, xPos, yPos);
    };

    glfwSetCursorPosCallback(window_, funcCoords);

}


void InputManager::updateMouseCoords(GLFWwindow* window, double xpos, double ypos)
{
    actionMouse_ = InputMouse::HOVER;
    coordinates_.x = xpos;
    coordinates_.y = ypos;
}

void InputManager::mouseButtonPressed(GLFWwindow* window, int button, int action , int mods)
{
    // left click
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
    {
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);

        actionMouse_ = InputMouse::PRESS;
        coordinates_.x = xpos;
        coordinates_.y = ypos;
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) 
    {
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);

        actionMouse_ = InputMouse::RELEASE;
        coordinates_.x = xpos;
        coordinates_.y = ypos;

    }
}



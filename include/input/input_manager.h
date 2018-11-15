#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

// std includes
#include <map>
#include <string>

// external libs includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

// project includes
#include "resources/singleton.h"
#include "input/event_handlers.h"
#include "input/input_mouse.h"

// GLFW3 defines 348 key codes
const int NUMBER_OF_KEY_CODES = 348;
const int NUMBER_OF_MOUSE_CODES = 1;

class InputManager: public Singleton<InputManager>
{
public:

    void startUp();
    void shutDown();

    void pollEvents(float delta);
    void processKeyEvent(int key, int scancode, int action, int mods);

     void window(GLFWwindow* window) { window_ = window; };

    void addKeyHandler(const std::string& name, KeyHandler keyHandler);
    void removeKeyHandler(const std::string& name);

    // Key input
    bool isKeyPressed(int key) { return keys_[key]; }
    bool isKeyProcessed(int key) { return processedKeys_[key]; }
    void setProcessedKey(int key) { processedKeys_[key] = true; }

    // Mouse input
    glm::vec2 coordinates() const { return coordinates_; };
    InputMouse actionMouse() const { return actionMouse_; };

private:
    // For key inpunts
    std::map<std::string, KeyHandler> keyHandlers_;
    bool keys_[NUMBER_OF_KEY_CODES];
    bool processedKeys_[NUMBER_OF_KEY_CODES];

    // For mouse inputs
    // Mouse coordinates
    glm::vec2 coordinates_;
    InputMouse actionMouse_;
    GLFWwindow* window_;

    void mouseHandler();
    void updateMouseCoords(GLFWwindow* window, double xpos, double ypos);
    void mouseButtonPressed(GLFWwindow* window, int button, int action , int mods);

};

#endif

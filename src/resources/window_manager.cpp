#include "resources/window_manager.h"

// std includes
#include <iostream>

// external libs includes
#include <GLFW/glfw3.h>

void WindowManager::startUp() 
{

    // Setup error handling
    glfwSetErrorCallback([](int errorCode, const char* description) -> void 
    {
        std::cerr << description << std::endl;
    });

    std::cout << glfwGetVersionString() << std::endl;

    if (!glfwInit()) 
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
    }

    // Using OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void WindowManager::shutDown() 
{
    for (auto& window : windows_) 
    {
        window->destroy();
    }

    glfwTerminate();
}

Window* WindowManager::createWindow(int width,
                                    int height,
                                    const std::string& title,
                                    bool isFullScreen,
                                    bool vsync) 
{
    // cannot use make_unique with private constructor even inside a friend class
    windows_.push_back(std::unique_ptr<Window>(new Window(width, height, title, isFullScreen, vsync)));

    return windows_.back().get();
}

#include "resources/window.h"

// std includes
#include <iostream>

// project includes
#include "input/input_manager.h"
#include "utils/file_manager.h"

Window::Window(int width, int height, const std::string& title, bool isFullScreen, bool vsync)
    : width_(width), height_(height), title_(title), isFullScreen_(isFullScreen), vsync_(vsync) 
{
    auto primaryMonitor = glfwGetPrimaryMonitor();

    window_ = isFullScreen ?
                  glfwCreateWindow(width, height, title.c_str(), primaryMonitor, nullptr) :
                  glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (window_ == nullptr) 
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
     }

    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (!isFullScreen) 
    {
        // Center window on screen
        auto vidMode = glfwGetVideoMode(primaryMonitor);
        glfwSetWindowPos(window_,
                         (vidMode->width - width) / 2,
                         (vidMode->height - height) / 2);
    }

    // Create OpenGL context
    this->makeContextCurrent();

    // Enable v-sync
    if (vsync_) 
    {
        glfwSwapInterval(1);
    }

    // Define the viewport dimensions
    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window_, &frameBufferWidth, &frameBufferHeight);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    this->setupEventHandlers();
}

Window::~Window() 
{
}

void Window::destroy() 
{
    glfwDestroyWindow(window_);
}

bool Window::isClosing() const
{
    return static_cast<bool>(glfwWindowShouldClose(window_));
}

void Window::setIsShouldClose(bool isShouldClose) 
{
    glfwSetWindowShouldClose(window_, isShouldClose);
}

void Window::makeContextCurrent() 
{
    glfwMakeContextCurrent(window_);
}

void Window::swapBuffers() 
{
    glfwSwapBuffers(window_);
}

void Window::setupEventHandlers() 
{
    glfwSetKeyCallback(window_, [](GLFWwindow* window,
                                int key,
                                int scancode,
                                int action,
                                int mods) 
                                { InputManager::Instance().processKeyEvent(key, scancode, action, mods);});
}

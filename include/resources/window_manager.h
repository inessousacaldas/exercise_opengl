#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

// std includes
#include <memory>
#include <vector>

// project includes
#include "resources/singleton.h"
#include "resources/window.h"

class WindowManager: public Singleton<WindowManager> 
{
public:
    void startUp();
    void shutDown();

    Window* createWindow(int width,
                         int height,
                         const std::string& title,
                         bool isFullScreen = false,
                         bool vsync = true);
private:
    std::vector<std::unique_ptr<Window>> windows_;
};

#endif

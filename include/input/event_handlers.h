#ifndef EVENT_HANDLERS_H
#define EVENT_HANDLERS_H

// std includes
#include <functional>

// external libs includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using KeyHandler = std::function<void (float delta)>;

#endif

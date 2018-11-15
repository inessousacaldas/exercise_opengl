// external libs includes
#include <GL/glew.h> // glew header must be included before glfw header
#include <GLFW/glfw3.h>

// project includes
#include "game/game.h"


const int width = 800;
const int height = 1000;
bool isFullScreen = false;

int main(int argc, char* argv[]) {
    Game breakout(width, height, isFullScreen);

    GLfloat delta;
    auto lastFrame = static_cast<GLfloat>(glfwGetTime());

    while (!breakout.isExiting()) {
        // Calculate delta time
        auto currentFrame = static_cast<GLfloat>(glfwGetTime());
        delta = currentFrame - lastFrame;
        lastFrame = currentFrame;

        breakout.input(delta);
        breakout.update(delta);
        breakout.render();
    }

    return EXIT_SUCCESS;
}

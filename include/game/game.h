#ifndef GAME_H
#define GAME_H

// std includes
#include <memory>
#include <vector>
#include <functional>

// external libs includes
#include <GL/glew.h>

// project includes
#include "game/game_state.h"
#include "game/game_level.h"
#include "game/power_up.h"
#include "game/player.h"
#include "game/ball.h"
#include "game/menu.h"

#include "graphics/particle_emitter.h"
#include "graphics/post_processor.h"
#include "graphics/sprite_renderer.h"
#include "graphics/text_renderer.h"

#include "audio/audio_manager.h"

#include "input/input_manager.h"

#include "resources/resource_manager.h"
#include "resources/window_manager.h"
#include "resources/window.h"

class Game {
public:
    Game(int width, int height, bool isFullScreen);
    ~Game();

    void input(GLfloat delta);
    void update(GLfloat delta);
    void render();

    void startButton();
    void increaseCreditsButton();
    void reduceCreditsButton();
    void updateCredits(GLint num = 1);

    void reset();

    bool isExiting() const;

private:

    // Window Setup
    Window* window_;
    glm::vec2 scales_;

    // Managers
    WindowManager& windowManager_ = WindowManager::Instance();
    InputManager& inputManager_ = InputManager::Instance();
    ResourceManager& resourceManager_ = ResourceManager::Instance();
    AudioManager& audioManager_ = AudioManager::Instance();

    // Renders
    SpriteRenderer& spriteRenderer_ = SpriteRenderer::Instance();
    TextRenderer& textRenderer_ = TextRenderer::Instance();

    // Game Objects
    std::unique_ptr<Player> player_;
    std::unique_ptr<Ball> ball_;
    std::unique_ptr<Menu> menu_;

    std::vector<std::shared_ptr<GameLevel>> levels_;
    std::vector<std::unique_ptr<PowerUp>> powerUps_;

    // Efects
    std::unique_ptr<ParticleEmitter> particleEmitter_;
    std::unique_ptr<PostProcessor> postProcessor_;
    GLfloat shakeTime_; // For postprocessing effect

    // Game State
    GameState gameState_;
    GLint lives_;
    GLint currentLevel_;
    bool animateLevel_;

    // Game menu
    GLint credits_;
    GLint creditsIn_; // count for number of credits added by the player
    GLint creditsOut_; // counter for number of credits removed by the player
    GLint numberOfgamesPlayed_;
    
    void initWindow(int width, int height, bool isFullScreen);
    void initGL();
    void initResources();
    void initMenu();

    void checkCollisions();
    void checkCollisionsWalls();
    void spawnPowerUps(const Brick& brick);
};

#endif

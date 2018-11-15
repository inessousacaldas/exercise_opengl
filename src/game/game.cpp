#include "game/game.h"

// std includes
#include <tuple>
#include <algorithm>
#include <iostream>

// external libs includes
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GLFW/glfw3.h"

// project includes
#include "graphics/shader.h"
#include "graphics/post_processing_effect.h"

#include "physics/collision.h"
#include "physics/direction.h"
#include "physics/collision_detector.h"

#include "utils/random.h"

#include "resources/window.h"
#include "resources/assets_loader.h"

#include "input/input_mouse.h"

const int GAME_WIDTH = 800;
const int GAME_HEIGHT = 1000;

const int LIVES = 3;
const int REWARD_LEVEL_WON = 5;

// Menu consts
const int MENU_HEIGHT = 200;
const int START_BUTTON_WIDTH = 102;
const int START_BUTTON_HEIGHT = 80;

const int CREDITS_BUTTON_WIDTH = 52;
const int CREDITS_BUTTON_HEIGHT = 80;

const int CREDITS_FRAME_WIDTH = 267;
const int CREDITS_FRAME_HEIGHT = 80;

const int CHAR_SIZE = 20;

// Setup game consts
const glm::vec2 INITIAL_BALL_VELOCITY(200.0f, -550.0f);
const float INITIAL_PLAYER_VELOCITY = 500.0f;
const glm::vec2 PLAYER_SIZE = glm::vec2(120, 46);
const float BALL_RADIUS = 8.0f;

Game::Game(int width, int height, bool isFullScreen)
    : gameState_(GameState::GAME_MENU), 
        lives_(LIVES), 
        scales_(static_cast<float>(width) / GAME_WIDTH, static_cast<float>(height) / GAME_HEIGHT),
        credits_(0), 
        creditsIn_(0), 
        creditsOut_(0),
        numberOfgamesPlayed_(0),
        animateLevel_(false)
{
    // Start managers
    windowManager_.startUp();
    inputManager_.startUp();
    resourceManager_.startUp();
    audioManager_.startUp();

    //Window setup
    this->initWindow(width, height, isFullScreen);
    this->initGL();
    this->initResources();
    inputManager_.window(window_->window());

    // Play menu music
    audioManager_.playSource("menu");
}

Game::~Game()
{ 
    menu_->destroy(); 
    audioManager_.shutDown();
    resourceManager_.shutDown();
    inputManager_.shutDown();
    windowManager_.shutDown();
}

void Game::input(GLfloat delta)
{
    inputManager_.pollEvents(delta);
    
    // Get mouse coordinates
    glm::vec2 coordinates = inputManager_.coordinates();

    // Check menu buttons actions
    menu_->hoverButtons(coordinates, inputManager_.actionMouse());

    switch(gameState_) 
    {

        case GameState::GAME_MENU:
        {
            // Start button behaviour
            if (inputManager_.isKeyPressed(GLFW_KEY_ENTER) && !inputManager_.isKeyProcessed(GLFW_KEY_ENTER))
            {
                inputManager_.setProcessedKey(GLFW_KEY_ENTER);
                startButton();
            } 
            // Chose level up
            else if (inputManager_.isKeyPressed(GLFW_KEY_W) && !inputManager_.isKeyProcessed(GLFW_KEY_W))
            {
                inputManager_.setProcessedKey(GLFW_KEY_W);
                currentLevel_ = (currentLevel_ + 1) % levels_.size();
            } 
            // Chose level up
            else if (inputManager_.isKeyPressed(GLFW_KEY_UP) && !inputManager_.isKeyProcessed(GLFW_KEY_UP))
            {
                inputManager_.setProcessedKey(GLFW_KEY_UP);
                currentLevel_ = (currentLevel_ + 1) % levels_.size();
            }
            // Chose level down
            else if (inputManager_.isKeyPressed(GLFW_KEY_S) && !inputManager_.isKeyProcessed(GLFW_KEY_S))
            {
                inputManager_.setProcessedKey(GLFW_KEY_S);
                currentLevel_ = currentLevel_ > 0 ? --currentLevel_ : levels_.size() - 1;
            }
            // Chose level down
            else if (inputManager_.isKeyPressed(GLFW_KEY_DOWN) && !inputManager_.isKeyProcessed(GLFW_KEY_DOWN))
            {
                inputManager_.setProcessedKey(GLFW_KEY_DOWN);
                currentLevel_ = currentLevel_ > 0 ? --currentLevel_ : levels_.size() - 1;
            }
            // Add credits
            else if (inputManager_.isKeyPressed(GLFW_KEY_RIGHT) && !inputManager_.isKeyProcessed(GLFW_KEY_RIGHT))
            {
                inputManager_.setProcessedKey(GLFW_KEY_RIGHT);
                increaseCreditsButton();
            }
            // Add credits
            else if (inputManager_.isKeyPressed(GLFW_KEY_D) && !inputManager_.isKeyProcessed(GLFW_KEY_D))
            {
                inputManager_.setProcessedKey(GLFW_KEY_D);
                increaseCreditsButton();
            }
            // Reduce credits
            else if (inputManager_.isKeyPressed(GLFW_KEY_LEFT) && !inputManager_.isKeyProcessed(GLFW_KEY_LEFT))
            {
                inputManager_.setProcessedKey(GLFW_KEY_LEFT);
                reduceCreditsButton();
            }
            // Reduce credits
            else if (inputManager_.isKeyPressed(GLFW_KEY_A) && !inputManager_.isKeyProcessed(GLFW_KEY_A))
            {
                inputManager_.setProcessedKey(GLFW_KEY_A);
                reduceCreditsButton();
            }
            break;
        }
        
        case GameState::GAME_ACTIVE: 
        {
            // Start button behaviour
            if (inputManager_.isKeyPressed(GLFW_KEY_ENTER) && !inputManager_.isKeyProcessed(GLFW_KEY_ENTER))
            {
                inputManager_.setProcessedKey(GLFW_KEY_ENTER);
                startButton();
            }

            // Control player inputs
            auto velocity = player_->velocity() * delta;

            if (inputManager_.isKeyPressed(GLFW_KEY_A) || inputManager_.isKeyPressed(GLFW_KEY_LEFT))
            {
                if (player_->position().x >= player_->boundaries().x)
                {
                    player_->updatePositionX(-velocity);
                    if (ball_->isStuck())
                    {
                        ball_->updatePositionX(-velocity);
                    }
                }
            }
            if (inputManager_.isKeyPressed(GLFW_KEY_D) || inputManager_.isKeyPressed(GLFW_KEY_RIGHT))
            {
                if (player_->position().x <= player_->boundaries().y)
                {
                    player_->updatePositionX(velocity);
                    if (ball_->isStuck())
                    {
                        ball_->updatePositionX(velocity);
                    }
                }
            }

            if (inputManager_.isKeyPressed(GLFW_KEY_SPACE))
            {
                ball_->isStuck(false);
            }
            break;
        }
        case GameState::GAME_PAUSE:
        {
            // Start button behaviour
            if (inputManager_.isKeyPressed(GLFW_KEY_ENTER) && !inputManager_.isKeyProcessed(GLFW_KEY_ENTER))
            {
                inputManager_.setProcessedKey(GLFW_KEY_ENTER);
                startButton();
            }

            break;
        }
        case GameState::GAME_WIN:
        case GameState::GAME_LOSE:
        {
            if (inputManager_.isKeyPressed(GLFW_KEY_ENTER) && !inputManager_.isKeyProcessed(GLFW_KEY_ENTER))
            {
                inputManager_.setProcessedKey(GLFW_KEY_ENTER);
                startButton();
            }
            break;
        }
    }
}

void Game::update(GLfloat delta)
{

    if(gameState_ == GameState::GAME_LOADING && animateLevel_)
    {
        levels_[currentLevel_]->initialAnimation();
        animateLevel_ = false;
    }
    else if(gameState_ == GameState::GAME_LOADING && levels_[currentLevel_]->isLoaded())
    {
        gameState_ = GameState::GAME_ACTIVE;
    }
    
    // Update postprocessing
    if (shakeTime_ > 0.0f) 
    {
        shakeTime_ -= delta;
        if (shakeTime_ <= 0.0f)
        {
            postProcessor_->disableEffects(PostProcessingEffect::Shake);
        }
    }
    
    // If not game active, game physic not updated
    if(gameState_ != GameState::GAME_ACTIVE)
    {
        return;
    }
    

    player_->boundaries(glm::vec2(0, window_->width() - player_->size().x));
    ball_->update(delta);
    if(gameState_ == GameState::GAME_ACTIVE)
    {
        particleEmitter_->update(delta, *ball_, 5, glm::vec2(ball_->radius() / 2));
    }
    
    this->checkCollisions();

    for (auto& powerUp: powerUps_)
    {
        powerUp->update(delta, *player_.get(), *ball_.get(), *postProcessor_.get());
    }
    powerUps_.erase(std::remove_if(powerUps_.begin(), powerUps_.end(),
            [](std::unique_ptr<PowerUp> const& powerUp) { return powerUp->isDestroyed() && !powerUp->isActivated(); }),
            powerUps_.end());

    if (gameState_ == GameState::GAME_ACTIVE && levels_[currentLevel_]->isCompleted()) // Game won
    {
        audioManager_.playSource("gameWon");

        this->reset();
        gameState_ = GameState::GAME_WIN;

        // Add credits reward
        updateCredits(REWARD_LEVEL_WON);
    }

    if (ball_->position().y >= window_->height() - ball_->size().y)
    {
        lives_--;
        if (lives_ == 0) // Game Over
        {
            audioManager_.playSource("gameOver");
            this->reset();
            gameState_ = GameState::GAME_LOSE;
        }
    }
}

void Game::render() {

  
    postProcessor_->beginRender();

    spriteRenderer_.renderSprite(resourceManager_.texture("background"),
        glm::vec2(0.0f, MENU_HEIGHT),
        glm::vec2(window_->width(), window_->height() - MENU_HEIGHT)
    );

    menu_->render(spriteRenderer_);

    levels_[currentLevel_]->render(spriteRenderer_);
    player_->render(spriteRenderer_);
    particleEmitter_->render(ball_->radius());
    ball_->render(spriteRenderer_);

    for (auto& powerUp : powerUps_)
    {
        if (!powerUp->isDestroyed())
        {
            powerUp->render(spriteRenderer_);
        }
    }

    textRenderer_.renderText("Lives: " + std::to_string(lives_),
        glm::vec2(10.0f, 50.f) * scales_,
        glm::vec3(0.1f), 0.6f * glm::length(scales_));

    textRenderer_.renderText("Credits In: " + std::to_string(creditsIn_),
        glm::vec2(10.0f, CHAR_SIZE * 0.3 + 100.0f) * scales_,
        glm::vec3(0.1f), 0.3f * glm::length(scales_));

    textRenderer_.renderText("Credits Out: " + std::to_string(creditsOut_),
        glm::vec2(10.0f, 6*CHAR_SIZE * 0.3 + 100.0f) * scales_,
        glm::vec3(0.1f), 0.3f * glm::length(scales_));

    textRenderer_.renderText("Games Played: " + std::to_string(numberOfgamesPlayed_),
        glm::vec2(10.0f, 11*CHAR_SIZE * 0.3 + 100.0f) * scales_,
        glm::vec3(0.1f), 0.3f * glm::length(scales_));


    glm::vec2 position = glm::vec2(200 + CREDITS_BUTTON_WIDTH, MENU_HEIGHT/2 + CHAR_SIZE/2) * scales_;
    glm::vec3 color = glm::vec3(0.0f);

    textRenderer_.renderText("$ " + std::to_string(credits_),
                            position,
                            color, 
                            0.5f * glm::length(scales_));

    postProcessor_->endRender();
    postProcessor_->render(static_cast<GLfloat>(glfwGetTime()), glm::length(scales_));


    std::string line;
    int offset;

    switch(gameState_) 
    {
        case GameState::GAME_MENU:
    
            line = std::string("Press START to play");
            offset = line.size() * CHAR_SIZE * 0.6;
            textRenderer_.renderText(line, 
                                    glm::vec2(GAME_WIDTH / 2 - offset, GAME_HEIGHT / 2 + MENU_HEIGHT) * scales_, 
                                    glm::vec3(0.1f), 0.6f * glm::length(scales_));

            line = std::string("Press W or S to select level");
            offset = line.size() * CHAR_SIZE * 0.4;
            textRenderer_.renderText(line, 
                                    glm::vec2(GAME_WIDTH / 2 - offset, GAME_HEIGHT / 2 + MENU_HEIGHT + 40) * scales_, 
                                    glm::vec3(0.1f), 0.4f * glm::length(scales_));

        break;

        case GameState::GAME_PAUSE:

            line = std::string("PAUSE");
            offset = line.size() * CHAR_SIZE * 0.6;
            textRenderer_.renderText(line, 
                                    glm::vec2(GAME_WIDTH / 2 - offset, GAME_HEIGHT / 2 + MENU_HEIGHT) * scales_, 
                                    glm::vec3(0.1f), 0.6f * glm::length(scales_));

        break;

        case GameState::GAME_WIN:

            line = std::string("You WON!!!");
            offset = line.size() * CHAR_SIZE * 0.7;
            textRenderer_.renderText(line, 
                                 glm::vec2(GAME_WIDTH / 2 - offset, GAME_HEIGHT / 2 + MENU_HEIGHT) * scales_, 
                                 glm::vec3(0.0f, 1.0f, 0.0f), 0.7f * glm::length(scales_));
            
            line = std::string("Press START to retry or ESC to quit");
            offset = line.size() * CHAR_SIZE * 0.4;
            textRenderer_.renderText(line, 
                                 glm::vec2(GAME_WIDTH / 2 - offset, GAME_HEIGHT / 2 + - MENU_HEIGHT + 20) * scales_, 
                                 glm::vec3(0.1f), 0.4f * glm::length(scales_));

        break;

        case GameState::GAME_LOSE:

            line = std::string("You LOST...");
            offset = line.size() * CHAR_SIZE * 0.7;
            textRenderer_.renderText(line, 
                                 glm::vec2(GAME_WIDTH / 2 - 110, GAME_HEIGHT / 2 + MENU_HEIGHT) * scales_, 
                                 glm::vec3(1.0f, 0.0f, 0.0f), 0.7f * glm::length(scales_));

            line = std::string("Press START to retry or ESC to quit");
            offset = line.size() * CHAR_SIZE * 0.4f;
            textRenderer_.renderText(line, 
                                 glm::vec2(GAME_WIDTH / 2 - 260, GAME_HEIGHT / 2 + MENU_HEIGHT + 20) * scales_, 
                                 glm::vec3(0.1f), 0.4f * glm::length(scales_));
        break;

    }

    window_->swapBuffers();
}

void Game::reset()
{
    levels_[currentLevel_]->reset();
    
    auto playerSize = PLAYER_SIZE * scales_;
    auto playerPosition = glm::vec2(
        window_->width() / 2 - playerSize.x / 2,
        window_->height() - playerSize.y
    );
    player_->size(playerSize);
    player_->position(playerPosition);
    player_->color(glm::vec3(1.0f));
    
    auto ballRadius = BALL_RADIUS * glm::length(scales_);
    ball_->position(playerPosition + glm::vec2(playerSize.x / 2 - ballRadius, -2 * ballRadius));
    ball_->color(glm::vec3(1.0f));
    ball_->velocity(INITIAL_BALL_VELOCITY * scales_);
    ball_->isStuck(true);
    ball_->isSticky(false);
    ball_->isPassingThrough(false);
    
    postProcessor_->disableEffects(PostProcessingEffect::Shake | 
                                   PostProcessingEffect::Blur);

    powerUps_.clear();

    particleEmitter_->reset();

    lives_ = LIVES;
}

bool Game::isExiting() const
{
    return window_->isClosing();
}

void Game::initWindow(int width, int height, bool isFullScreen)
{
    window_ = windowManager_.createWindow(width, height, "Breakout", isFullScreen);

    inputManager_.addKeyHandler("exit", [this](float delta) 
    {
        if (inputManager_.isKeyPressed(GLFW_KEY_ESCAPE)) 
        {
            window_->setIsShouldClose(true);
        }
    });
}

void Game::initGL()
{

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::initResources()
{
    AssetsLoader::loadShaders();

    auto spriteShader = resourceManager_.shader("sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(window_->width()),
                                      static_cast<GLfloat>(window_->height()), 0.0f,
                                      -1.0f, 1.0f);
    spriteShader->use();
    spriteShader->setUniform("projection", projection);
    spriteShader->setUniform("sprite", 0);

    spriteRenderer_.init(spriteShader);

    auto textRenderingShader = resourceManager_.shader("text");
    textRenderingShader->use();
    textRenderingShader->setUniform("projection", projection);
    textRenderer_.init(AssetsLoader::RESOURCES_FOLDER + "fonts/PixelScreen.ttf", textRenderingShader);

    auto particleShader = resourceManager_.shader("particle");
    particleShader->use();
    particleShader->setUniform("projection", projection);
    particleShader->setUniform("sprite", 0);

    AssetsLoader::loadTextures();
    AssetsLoader::loadAudioFiles();

    particleEmitter_ = std::make_unique<ParticleEmitter>(resourceManager_.shader("particle"),
                                                         resourceManager_.texture("particle"),
                                                         500);
    
    postProcessor_ = std::make_unique<PostProcessor>(resourceManager_.shader("postprocessing"),
                                                     window_->width(), window_->height(), MENU_HEIGHT);

    levels_.push_back(std::make_shared<GameLevel>(
        AssetsLoader::RESOURCES_FOLDER + "levels/1.txt", window_->width(), window_->height()/2  - MENU_HEIGHT));

    levels_.push_back(std::make_shared<GameLevel>(
        AssetsLoader::RESOURCES_FOLDER + "levels/2.txt", window_->width(), window_->height()/2  - MENU_HEIGHT));

    levels_.push_back(std::make_shared<GameLevel>(
        AssetsLoader::RESOURCES_FOLDER + "levels/3.txt", window_->width(), window_->height()/2  - MENU_HEIGHT));

    levels_.push_back(std::make_shared<GameLevel>(
        AssetsLoader::RESOURCES_FOLDER + "levels/4.txt", window_->width(), window_->height()/2  - MENU_HEIGHT));

    levels_.push_back(std::make_shared<GameLevel>(
        AssetsLoader::RESOURCES_FOLDER + "levels/5.txt", window_->width(), window_->height()/2  - MENU_HEIGHT));
    
    currentLevel_ = 0;

    auto playerSize = PLAYER_SIZE * scales_;
    auto playerPosition = glm::vec2(
        window_->width() / 2 - playerSize.x / 2,
        window_->height() - playerSize.y
    );

    player_ = std::make_unique<Player>(playerPosition,
                                       playerSize,
                                       glm::vec3(1.0f),
                                       resourceManager_.texture("paddle"),
                                       INITIAL_PLAYER_VELOCITY * scales_.x,
                                       glm::vec2(0, window_->width() - playerSize.x));

    auto ballRadius = BALL_RADIUS * glm::length(scales_);

    ball_ = std::make_unique<Ball>(playerPosition + glm::vec2(playerSize.x / 2 - ballRadius, -2 * ballRadius),
                                   ballRadius,
                                   glm::vec3(1.0f),
                                   resourceManager_.texture("ball"),
                                   INITIAL_BALL_VELOCITY * scales_,
                                   glm::vec4(0.0f, window_->width(), MENU_HEIGHT, window_->height()));

    initMenu();

}


void Game::initMenu()
{
    // Define position and size of objects
    glm::vec2 position = glm::vec2(0, 0);
    glm::vec2 size = glm::vec2(window_->width(), MENU_HEIGHT);

    // Menu background
    menu_ = std::make_unique<Menu>(position,
                                    size,
                                    glm::vec3(1.0f),
                                    resourceManager_.texture("menu"));
    
    // Add start button
    position = glm::vec2(window_->width() - 100 - START_BUTTON_WIDTH, MENU_HEIGHT/2 - START_BUTTON_HEIGHT / 2);
    size = glm::vec2(START_BUTTON_WIDTH, START_BUTTON_HEIGHT);
    
    std::function<void()> startCalbackFun;
    Game* game;
    startCalbackFun = std::bind(&Game::startButton, this);

    menu_->addButton("play", 
                    position,
                    size,
                    glm::vec3(1.0f),
                    resourceManager_.texture("button_play"),
                    resourceManager_.texture("button_play_focus"),
                    resourceManager_.texture("button_play_pressed"), startCalbackFun);
    
    // Add minus button
    position = glm::vec2(180, MENU_HEIGHT/2 - CREDITS_BUTTON_HEIGHT / 2);
    size = glm::vec2(CREDITS_BUTTON_WIDTH, CREDITS_BUTTON_HEIGHT);
 
    std::function<void()> reduceCreditsCalbackFun;
    reduceCreditsCalbackFun = std::bind(&Game::reduceCreditsButton, this);
    
     menu_->addButton("minus", 
                    position,
                    size,
                    glm::vec3(1.0f),
                    resourceManager_.texture("button_minus"),
                    resourceManager_.texture("button_minus_focus"),
                    resourceManager_.texture("button_minus_pressed"), reduceCreditsCalbackFun);

    // Add plus button
    position = glm::vec2(180 + CREDITS_FRAME_WIDTH - CREDITS_BUTTON_WIDTH, MENU_HEIGHT/2 - CREDITS_BUTTON_HEIGHT / 2);
    size = glm::vec2(CREDITS_BUTTON_WIDTH, CREDITS_BUTTON_HEIGHT);
 
    std::function<void()> increaseCreditsCalbackFun;
    increaseCreditsCalbackFun = std::bind(&Game::increaseCreditsButton, this);
    
     menu_->addButton("plus", 
                    position,
                    size,
                    glm::vec3(1.0f),
                    resourceManager_.texture("button_plus"),
                    resourceManager_.texture("button_plus_focus"),
                    resourceManager_.texture("button_plus_pressed"), increaseCreditsCalbackFun);

    // Credits Frame
    position = glm::vec2(180, MENU_HEIGHT/2 - CREDITS_BUTTON_HEIGHT / 2);
    size = glm::vec2(CREDITS_FRAME_WIDTH, CREDITS_FRAME_HEIGHT);

    menu_->addCreditsFrame(position,
                        size,
                        glm::vec3(1.0f),
                        resourceManager_.texture("credits_frame"));

}

void Game::startButton()
{
    audioManager_.playSource("click");

    if(gameState_ == GameState::GAME_MENU && credits_ > 0) // start game
    {
        // Update variables
        updateCredits(-1);
        numberOfgamesPlayed_ += 1;

        // Update game state
        gameState_ = GameState::GAME_LOADING;
        levels_[currentLevel_]->initialAnimation();

        // Change music
        audioManager_.stopSource("menu");
        audioManager_.playSource("game");

    }
    else if(gameState_ == GameState::GAME_MENU && credits_ == 0) // not enough credits to start the game
    {
        audioManager_.playSource("solid");
        shakeTime_ = 0.2f;
        postProcessor_->enableEffects(PostProcessingEffect::Shake);
    }
    else if(gameState_ == GameState::GAME_ACTIVE)
    {
        gameState_ = GameState::GAME_PAUSE;
        postProcessor_->enableEffects(PostProcessingEffect::Blur);

    }
    else if(gameState_ == GameState::GAME_PAUSE)
    {
        gameState_ = GameState::GAME_ACTIVE;
        postProcessor_->disableEffects(PostProcessingEffect::Blur);

    }
    else if(gameState_ == GameState::GAME_WIN || gameState_ == GameState::GAME_LOSE)
    {
        gameState_ = GameState::GAME_MENU;

         // Change music
        audioManager_.stopSource("game");
        audioManager_.playSource("menu");
    }
}

void Game::increaseCreditsButton() 
{
    audioManager_.playSource("click");

    credits_ += 1;
    creditsIn_ += 1;
}

void Game::reduceCreditsButton() 
{
    audioManager_.playSource("click");

    if(credits_ > 0)
    {
        credits_ -= 1;
        creditsOut_ -= 1;
    }
}

void Game::updateCredits(GLint num)
{
    credits_ += num;

    if(credits_ < 0)
    {
        credits_ = 0;
    }
}

void Game::checkCollisions() 
{
    // check collisions with the bricks
    for (auto& brick : levels_[currentLevel_]->bricks())
    {
        if (brick->isDestroyed()) continue; // brick is destroyed

        auto collision = CollisionDetector::checkCollision(*ball_, *brick.get());
        if (!std::get<0>(collision)) continue; // if there is no collision...

        if (brick->isSolid())
        {
            audioManager_.playSource("solid");
            shakeTime_ = 0.2f;
            postProcessor_->enableEffects(PostProcessingEffect::Shake);
        } else
        {
            audioManager_.playSource("bleep");
            brick->isDestroyed(true);
            this->spawnPowerUps(*brick);
        }

        if (ball_->isPassingThrough()) continue;

        // Collision resolution
        auto direction = std::get<1>(collision);
        auto difference = std::get<2>(collision);

        if (direction == Direction::LEFT || direction == Direction::RIGHT)
        {
			ball_->velocityX(-ball_->velocity().x); // Reverse horizontal velocity
            // Relocate
            auto intersection = ball_->radius() - std::abs(difference.x);
            if (direction == Direction::LEFT)
            {
				ball_->updatePositionX(intersection); // Move ball to right
            } else
            {
				ball_->updatePositionX(-intersection); // Move ball to left
            }
        } else
        {
            ball_->velocityY(-ball_->velocity().y);
            auto intersection = ball_->radius() - std::abs(difference.y);
            if (direction == Direction::UP)
            {
				ball_->updatePositionY(-intersection); // Move ball back up
            } else
            {
                ball_->updatePositionY(intersection); // Move ball back down
            }
        }
    }

    // check collision with the paddle
    auto collision = CollisionDetector::checkCollision(*ball_, *player_);
    
    if (std::get<0>(collision) && !ball_->isStuck())
    {
        audioManager_.playSource("bleepPaddle");
        // Check where it hit the board, and change velocity based on where it hit the board
        auto centerBoard = player_->position().x + player_->size().x / 2;
        auto distance = (ball_->position().x + ball_->radius()) - centerBoard;
        auto percentage = distance / (player_->size().x / 2);

        // Then move accordingly
        auto strength = 2.0f;
        glm::vec2 oldVelocity = ball_->velocity();
        ball_->velocityX(INITIAL_BALL_VELOCITY.x * scales_.x * percentage * strength);
        ball_->velocityY(-1 * std::abs(ball_->velocity().y));
        ball_->velocity(glm::normalize(ball_->velocity()) * glm::length(oldVelocity));

        ball_->isStuck(ball_->isSticky());
    }

    // check collision between powerups and the paddle
    for (auto& powerup : powerUps_)
    {
        if (powerup->isDestroyed()) continue;

        if (powerup->position().y >= window_->height())
        {
            powerup->isDestroyed(true);
        }

        if (CollisionDetector::checkCollision(*player_, *powerup.get()))
        {
            audioManager_.playSource("powerup");
            powerup->activate(*player_.get(), *ball_.get(), *postProcessor_.get(), scales_);
        }
    }
}

void Game::spawnPowerUps(const Brick& brick)
{
    auto size = glm::vec2(40, 40) * scales_;
    auto velocity = glm::vec2(0.0f, 150.0f) * scales_;

    if (Random::chance(10)) 
    { 
        powerUps_.push_back(std::make_unique<PowerUp>(brick.position(), size, glm::vec3(0.5f, 0.5f, 1.0f), 
            resourceManager_.texture("powerUp"), velocity, PowerUpType::SpeedUp, 0.0f));
    }
    if (Random::chance(10)) 
    {
        powerUps_.push_back(std::make_unique<PowerUp>(brick.position(), size, glm::vec3(1.0f, 0.5f, 1.0f), 
            resourceManager_.texture("powerUp"), velocity, PowerUpType::Sticky, 5.0f));
    }
    if (Random::chance(10)) 
    {
        powerUps_.push_back(std::make_unique<PowerUp>(brick.position(), size, glm::vec3(0.5f, 1.0f, 0.5f), 
            resourceManager_.texture("powerUp"), velocity, PowerUpType::PassThrough, 6.0f));
    }
    if (Random::chance(90)) 
    {
        powerUps_.push_back(std::make_unique<PowerUp>(brick.position(), size, glm::vec3(1.0f, 0.6f, 0.4), 
            resourceManager_.texture("powerUp"), velocity, PowerUpType::PadSizeIncrease, 10.0f));
    }
}

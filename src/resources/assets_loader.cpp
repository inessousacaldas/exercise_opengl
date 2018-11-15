#include "resources/assets_loader.h"

// external libs includes
#include <GL/glew.h>

#include "resources/resource_manager.h"
#include "audio/audio_manager.h"

const std::string AssetsLoader::RESOURCES_FOLDER = "../resources/";

void AssetsLoader::loadShaders() 
{
    // Sprite - for textures
    ResourceManager::Instance().createShader("sprite",
         ShaderData(ShaderType::VERTEX,
                RESOURCES_FOLDER + "shaders/sprite/shader.vert"),
         ShaderData(ShaderType::FRAGMENT,
                RESOURCES_FOLDER + "shaders/sprite/shader.frag"));

    // Text
    ResourceManager::Instance().createShader("text",
         ShaderData(ShaderType::VERTEX,
                RESOURCES_FOLDER + "shaders/textrendering/shader.vert"),
         ShaderData(ShaderType::FRAGMENT,
                RESOURCES_FOLDER + "shaders/textrendering/shader.frag"));

    // Particle
    ResourceManager::Instance().createShader("particle",
         ShaderData(ShaderType::VERTEX,
                RESOURCES_FOLDER + "shaders/particle/shader.vert"),
         ShaderData(ShaderType::FRAGMENT,
                RESOURCES_FOLDER + "shaders/particle/shader.frag"));

    // Post processing effects
    ResourceManager::Instance().createShader("postprocessing",
         ShaderData(ShaderType::VERTEX,
                RESOURCES_FOLDER + "shaders/postprocessing/shader.vert"),
         ShaderData(ShaderType::FRAGMENT,
                RESOURCES_FOLDER + "shaders/postprocessing/shader.frag"));
}

void AssetsLoader::loadTextures() 
{
    // Background
    ResourceManager::Instance().createTexture("background",
       RESOURCES_FOLDER + "textures/background.png",
       1920, 1080);
    
    // Ball
    ResourceManager::Instance().createTexture("ball",
       RESOURCES_FOLDER + "textures/ball.png",
       48, 48, 4, GL_RGBA);

    // Blocks
    ResourceManager::Instance().createTexture("block1",
       RESOURCES_FOLDER + "textures/bricks/brick_1.png",
       384, 128, 4, GL_RGBA);
    ResourceManager::Instance().createTexture("block2",
        RESOURCES_FOLDER + "textures/bricks/brick_1_d.png",
        384, 128, 4, GL_RGBA);
    ResourceManager::Instance().createTexture("block_solid",
       RESOURCES_FOLDER + "textures/bricks/block.png",
        384, 128, 4, GL_RGBA);
    
    // Paddle
    ResourceManager::Instance().createTexture("paddle",
       RESOURCES_FOLDER + "textures/paddle.png",
       120, 46, 4, GL_RGBA);

    // Particle
    ResourceManager::Instance().createTexture("particle",
       RESOURCES_FOLDER + "textures/particle.png",
       58, 58, 4, GL_RGBA);

    // Power Ups
    ResourceManager::Instance().createTexture("powerUp",
        RESOURCES_FOLDER + "textures/powerup.png",
        256, 256, 4, GL_RGBA);


    // ----- Menu Textures -----

    // Background
    ResourceManager::Instance().createTexture("menu",
        RESOURCES_FOLDER + "textures/menu/menu2.jpg",
        800, 200);

    // Credits Frame
    ResourceManager::Instance().createTexture("credits_frame",
        RESOURCES_FOLDER + "textures/menu/credits_frame.png",
        737, 221, 4, GL_RGBA);

    // Start Button
    ResourceManager::Instance().createTexture("button_play",
        RESOURCES_FOLDER + "textures/menu/button_play.png",
        283, 222, 4, GL_RGBA);
    ResourceManager::Instance().createTexture("button_play_focus",
        RESOURCES_FOLDER + "textures/menu/button_play_focus2.png",
        283, 222, 4, GL_RGBA);
    ResourceManager::Instance().createTexture("button_play_pressed",
        RESOURCES_FOLDER + "textures/menu/button_play_pressed.png",
        283, 222, 4, GL_RGBA);

    // Minus Button
    ResourceManager::Instance().createTexture("button_minus",
        RESOURCES_FOLDER + "textures/menu/button_minus.png",
        145, 222, 4, GL_RGBA);
    ResourceManager::Instance().createTexture("button_minus_focus",
        RESOURCES_FOLDER + "textures/menu/button_minus_focus.png",
        145, 222, 4, GL_RGBA);
    ResourceManager::Instance().createTexture("button_minus_pressed",
        RESOURCES_FOLDER + "textures/menu/button_minus_pressed.png",
        145, 222, 4, GL_RGBA);

    // Plus Button
    ResourceManager::Instance().createTexture("button_plus",
        RESOURCES_FOLDER + "textures/menu/button_plus.png",
        145, 222, 4, GL_RGBA);
    ResourceManager::Instance().createTexture("button_plus_focus",
        RESOURCES_FOLDER + "textures/menu/button_plus_focus.png",
        145, 222, 4, GL_RGBA);
    ResourceManager::Instance().createTexture("button_plus_pressed",
        RESOURCES_FOLDER + "textures/menu/button_plus_pressed.png",
        145, 222, 4, GL_RGBA);
    
}

void AssetsLoader::loadAudioFiles() 
{
    // Music
    AudioManager::Instance().createSource("menu", RESOURCES_FOLDER + "audio/menu.ogg", true);
    AudioManager::Instance().createSource("game", RESOURCES_FOLDER + "audio/game.ogg", true);

    // SFX
    AudioManager::Instance().createSource("bleep", RESOURCES_FOLDER + "audio/bleep.ogg", false);
    AudioManager::Instance().createSource("bleepPaddle", RESOURCES_FOLDER + "audio/bleep_paddle.ogg", false);
    AudioManager::Instance().createSource("solid", RESOURCES_FOLDER + "audio/solid.ogg", false);
    AudioManager::Instance().createSource("powerup", RESOURCES_FOLDER + "audio/powerup.ogg", false);
    AudioManager::Instance().createSource("gameWon", RESOURCES_FOLDER + "audio/game-won.ogg", false);
    AudioManager::Instance().createSource("gameOver", RESOURCES_FOLDER + "audio/game-over.ogg", false);
    AudioManager::Instance().createSource("click", RESOURCES_FOLDER + "audio/click.ogg", false);
}

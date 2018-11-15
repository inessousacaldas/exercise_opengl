#include "game/game_level.h"

// std includes
#include <cstdlib>
#include <sstream>
#include <iostream>

// external libs includes
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

//project includes
#include "utils/file_manager.h"
#include "resources/resource_manager.h"

GameLevel::GameLevel(const std::string& path, int levelWidth, int levelHeight) 
{
    auto tiles = this->loadTiles(path);
    this->initLevel(tiles, levelWidth, levelHeight);
}

GameLevel::~GameLevel() {}

void GameLevel::render(SpriteRenderer& renderer) 
{
    for (auto& brick: bricks_) 
    {
        if (!brick->isDestroyed()) 
        {
            brick->render(renderer);
        }
    }
}

void GameLevel::reset()
{
    for (auto& brick: bricks_) 
    {
        brick->isDestroyed(false);
    }
}

bool GameLevel::isCompleted() 
{
    for (auto& brick: bricks_) 
    {
        if (!brick->isSolid() && !brick->isDestroyed()) return false;
    }

    return true;
}


bool GameLevel::isLoaded()
{
    for (auto& brick: bricks_)
    {
        if (brick->isAnimating()) // if any brick is animated, not finished
        {
            return false;
        } 
    }

    return true;
}

std::vector<std::vector<int>> GameLevel::loadTiles(const std::string& path) const
{
    std::vector<std::vector<int>> tiles;

    int tile;
    std::stringstream level(FileManager::readAsText(path));
    std::string line;
    while (std::getline(level, line)) 
    {
        std::istringstream rowStream(line);
        std::vector<int> row;
        while(rowStream >> tile) 
        {
            row.push_back(tile);
        }
        tiles.push_back(row);
    }

    return tiles;
}

void GameLevel::initLevel(std::vector<std::vector<int>>& tiles, int levelWidth, int levelHeight) 
{
    auto tileWidth = static_cast<float>(levelWidth) / tiles[0].size();
    auto tileHeight = static_cast<float>(levelHeight) / tiles.size();

    for (auto i = 0; i < tiles.size(); ++i) 
    {
        for (auto j = 0; j < tiles[0].size(); ++j) 
        {
            glm::vec2 position(tileWidth * j, tileHeight * i);
            glm::vec2 size(tileWidth, tileHeight);
            position += glm::vec2(0, 200);
            switch (tiles[i][j]) 
            {
                case 1:
                    bricks_.push_back(std::make_unique<Brick>(position, size, glm::vec3(1.0f, 1.0f, 1.0f),
                        ResourceManager::Instance().texture("block_solid"), true));
                    break;
                case 2:
                    bricks_.push_back(std::make_unique<Brick>(position, size, glm::vec3(1.0f, 1.0f, 0.1f),
                            ResourceManager::Instance().texture("block1"), false));
                    break;
                case 3:
                    bricks_.push_back(std::make_unique<Brick>(position, size, glm::vec3(1.0f, 0.65f, 0.0f),
                            ResourceManager::Instance().texture("block1"), false));
                    break;
                case 4:
                    bricks_.push_back(std::make_unique<Brick>(position, size, glm::vec3(0.13f, 0.55f, 0.13f),
                            ResourceManager::Instance().texture("block1"), false));
                    break;
                case 5:
                    bricks_.push_back(std::make_unique<Brick>(position, size, glm::vec3(0.1f, 0.1f, 0.45f),
                            ResourceManager::Instance().texture("block1"), false));
                    break;
                default:
                    break;
            }
        }
    }
}


void GameLevel::initialAnimation()
{
    double delay = 0.0;
    for (auto& brick: bricks_) 
    {
        if (!brick->isSolid())
        {
            brick->startAnimation(delay);
            delay += 0.005;
        }
    }
}
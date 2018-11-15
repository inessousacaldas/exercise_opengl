#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

// std includes
#include <string>
#include <memory>
#include <vector>

// project includes
#include "graphics/sprite_renderer.h"
#include "brick.h"

class GameLevel 
{
public:
    GameLevel(const std::string& path, int levelWidth, int levelHeight);
    ~GameLevel();

    void render(SpriteRenderer& renderer);
    void reset();
    bool isCompleted();
    bool isLoaded();

    void initialAnimation(); // starts the sequential animation of the bricks

    // getters
    std::vector<std::unique_ptr<Brick>>& bricks() { return bricks_; }
    
private:
    std::vector<std::unique_ptr<Brick>> bricks_;

    std::vector<std::vector<int>> loadTiles(const std::string& path) const;
    void initLevel(std::vector<std::vector<int>>& tiles, int levelWidth, int levelHeight);
};

#endif
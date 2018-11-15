#include "resources/resource_manager.h"

// std includes
#include <memory>

// project includes
#include "graphics/shader.h"
#include "graphics/shader_data.h"
#include "graphics/texture.h"

#include "utils/file_manager.h"

void ResourceManager::startUp() 
{
}

void ResourceManager::shutDown() 
{
    
    for (auto& shader : shaders_) 
    {
        shader.second->destroy();
    }
    for (auto& texture : textures_) 
    {
        texture.second->destroy();
    }
}

Shader* ResourceManager::createShader(const std::string& name,
                                                    const ShaderData& vertexShader,
                                                    const ShaderData& fragmentShader) 
{
    shaders_[name] = std::unique_ptr<Shader>(
        new Shader(vertexShader, fragmentShader)
    );

    return shaders_[name].get();
}

Texture* ResourceManager::createTexture(const std::string& name,
                                        const std::string& path,
                                        GLuint width,
                                        GLuint height,
                                        GLint channels,
                                        GLuint format) 
{
    auto image = FileManager::readImage(path, width, height, channels);

    textures_[name] = std::unique_ptr<Texture>(
        new Texture(width, height, image, format)
    );

    return textures_[name].get();
}

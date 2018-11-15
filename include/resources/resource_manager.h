#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

// std includes
#include <memory>
#include <map>
#include <string>

// external libs includes
#include <GL/glew.h>

// project includes
#include "resources/singleton.h"

#include "graphics/shader.h"
#include "graphics/shader_data.h"
#include "graphics/texture.h"

class ResourceManager: public Singleton<ResourceManager>
{
public:
    void startUp();
    void shutDown();

    Shader* createShader(const std::string& name,
                                       const ShaderData& vertexShader,
                                       const ShaderData& fragmentShader);
    Texture* createTexture(const std::string& name,
                           const std::string& path,
                           GLuint width,
                           GLuint height,
                           GLint channels = 3,
                           GLuint format = GL_RGB);

    // getters
    Shader* shader(const std::string& name) {
        return shaders_[name].get();
    }

    Texture* texture(const std::string& name) {
        return textures_[name].get();
    }

private:
    std::map<std::string, std::unique_ptr<Shader>> shaders_;
    std::map<std::string, std::unique_ptr<Texture>> textures_;
};

#endif
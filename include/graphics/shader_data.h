#ifndef SHADER_DATA_H
#define SHADER_DATA_H

// std includes
#include <string>

// external libs includes
#include <GL/glew.h>

// project includes
#include "graphics/shader_type.h"

class ShaderData
{
public:
    ShaderData(ShaderType type, const std::string& path);
    ~ShaderData();

    // getters
    GLuint id() const { return id_; };

private:
    GLuint id_;

    void checkCompilationStatus() const;
};

#endif

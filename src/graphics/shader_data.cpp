#include "graphics/shader_data.h"

// std includes
#include <iostream>
#include <string>
#include <vector>

// external libs includes
#include <GL/glew.h>

// project includes
#include "graphics/shader_type.h"
#include "utils/file_manager.h"

ShaderData::ShaderData(ShaderType type, const std::string& path) 
{
    auto source = FileManager::readAsText(path);

    id_ = glCreateShader(type);

    auto shaderSource = source.c_str();
    glShaderSource(id_, 1, &shaderSource, nullptr);
    glCompileShader(id_);

    this->checkCompilationStatus();
}

ShaderData::~ShaderData() 
{
    glDeleteShader(id_);
}

void ShaderData::checkCompilationStatus() const 
{
    GLint success;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        GLint LOG_LENGTH;
        glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &LOG_LENGTH);

        std::vector<GLchar> errorLog(static_cast<GLuint>(LOG_LENGTH));

        glGetShaderInfoLog(id_, LOG_LENGTH, nullptr, &errorLog[0]);
        std::cerr << "Shader compilation failed" << std::endl << &errorLog[0] << std::endl;
    }
}

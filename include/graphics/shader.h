#ifndef SHADER_H
#define SHADER_H

// std includes
#include <string>

// external libs includes
#include <GL/glew.h>
#include <glm/glm.hpp>

// project includes
#include "graphics/shader_data.h"

class Shader 
{
public:
    ~Shader();

    void use();
    void end();

    void setUniform(const std::string& name, GLboolean value);
    void setUniform(const std::string& name, GLint value);
    void setUniform(const std::string& name, GLuint value);
    void setUniform(const std::string& name, GLfloat value);
    void setUniform(const std::string& name, const glm::vec2& value);
    void setUniform(const std::string& name, const glm::vec3& value);
    void setUniform(const std::string& name, const glm::vec4& value);
    void setUniform(const std::string& name, const glm::mat2& value);
    void setUniform(const std::string& name, const glm::mat3& value);
    void setUniform(const std::string& name, const glm::mat4& value);
    void setUniform(const std::string& name, GLsizei count, GLboolean* values);
    void setUniform(const std::string& name, GLsizei count, GLint* values);
    void setUniform(const std::string& name, GLsizei count, GLuint* values);
    void setUniform(const std::string& name, GLsizei count, GLfloat* values);
    void setUniform(const std::string& name, GLsizei count, const glm::vec2* values);
    void setUniform(const std::string& name, GLsizei count, const glm::vec3* values);
    void setUniform(const std::string& name, GLsizei count, const glm::vec4* values);
    void setUniform(const std::string& name, GLsizei count, const glm::mat2* values);
    void setUniform(const std::string& name, GLsizei count, const glm::mat3* values);
    void setUniform(const std::string& name, GLsizei count, const glm::mat4* values);
private:
    GLuint id_;

    Shader(const ShaderData& vertexShader, const ShaderData& fragmentShader);
    void destroy();

    void checkLinkageStatus() const;
    GLint getUniformLocation(const std::string& name) const;

friend class ResourceManager;
};

#endif

#ifndef TEXTURE_H
#define TEXTURE_H

// external libs includes
#include <GL/glew.h>

class Texture 
{
public:
    ~Texture();
    void bind() const;
    void bind(GLuint unit) const;
    void unbind() const;

    void destroy();
private:
    GLuint id_;
    GLuint width_;
    GLuint height_;
    GLuint format_;

    Texture(GLuint width, GLuint height, unsigned char* image, GLuint format);
    

friend class ResourceManager;
};

#endif
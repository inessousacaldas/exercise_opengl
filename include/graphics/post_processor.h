#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

// external libs includes
#include <GL/glew.h>

// project includes
#include "graphics/shader.h"
#include "graphics/texture.h"

class PostProcessor 
{

public:
    PostProcessor(Shader* shader, GLuint width, GLuint height, GLuint menuHeight);
    ~PostProcessor();

    // Prepares the postprocessor's framebuffer operations before rendering the game
    void beginRender() const;
    // Should be called after rendering the game, so it stores all the rendered data into a texture object
    void endRender() const;
    // Renders the PostProcessor texture quad (as a screen-encompassing large sprite)
    void render(GLfloat delta, GLfloat scale);

    void enableEffects(int effects);
    void disableEffects(int effects);
    
private:
    Shader* shader_;
    Texture* texture_;

    GLuint width_;
    GLuint height_;
    GLuint menuHeight_;

    int effects_;

    GLuint MSFBO_;      // Multisampled Framebuffer Object
    GLuint FBO_;        // Framebuffer Object
    GLuint RBO_;        // Renderbuffer Object
    GLuint VAO_;        // Vertex Array Object
    GLuint VBO_;        // Vertex Buffer Object
    GLuint textureId_;
    GLuint textureId2_;

    void initFBOs();
    void initTexture();
    void initVAO();
    void initShaderUniforms();
};

#endif
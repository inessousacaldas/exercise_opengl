#include "graphics/post_processor.h"

// std includes
#include <stdexcept>
#include <iostream>

// external libs includes
#include <glm/vec2.hpp>

// project includes
#include "graphics/post_processing_effect.h"

PostProcessor::PostProcessor(Shader *shader, GLuint width, GLuint height, GLuint menuHeight)
    : shader_(shader), width_(width), height_(height), menuHeight_(menuHeight), effects_(0) 
{

    this->initFBOs();
    this->initVAO();
    this->initShaderUniforms();
}

PostProcessor::~PostProcessor()
{
    glDeleteFramebuffers(1, &MSFBO_);
    glDeleteFramebuffers(1, &FBO_);
    glDeleteRenderbuffers(1, &RBO_);
    glDeleteTextures(1, &textureId_);
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
}

void PostProcessor::beginRender() const 
{
    glBindFramebuffer(GL_FRAMEBUFFER, MSFBO_);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::endRender() const 
{
    // Now resolve multisampled color-buffer into intermediate FBO to store to texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, MSFBO_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO_);
    glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    // Binds both READ and WRITE framebuffer to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::render(GLfloat time, GLfloat scale) 
{
    shader_->use();
    shader_->setUniform("time", time);
    shader_->setUniform("scale", scale);
    shader_->setUniform("height", height_ - menuHeight_);
    shader_->setUniform("shake", effects_ & PostProcessingEffect::Shake);
    shader_->setUniform("blur", effects_ & PostProcessingEffect::Blur);

    // Render textured quad
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId_);
    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void PostProcessor::enableEffects(int effects)
 {
    if (effects & PostProcessingEffect::Shake) effects_ |= PostProcessingEffect::Shake;
    if (effects & PostProcessingEffect::Blur) effects_ |= PostProcessingEffect::Blur;
}

void PostProcessor::disableEffects(int effects) 
{
    if (effects & PostProcessingEffect::Shake) effects_ &= ~PostProcessingEffect::Shake;
    if (effects & PostProcessingEffect::Blur) effects_  &= ~PostProcessingEffect::Blur;
}

void PostProcessor::initFBOs() 
{
    // Initialize framebuffer/renderbuffer objects
    glGenFramebuffers(1, &MSFBO_);
    glGenFramebuffers(1, &FBO_);
    glGenRenderbuffers(1, &RBO_);

    // Initialize renderbuffer storage with a multisampled color buffer (don't need a depth/stencil buffer)
    glBindFramebuffer(GL_FRAMEBUFFER, MSFBO_);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO_);
    // Allocate storage for render buffer object
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGBA, width_, height_);
    // Attach MS render buffer object to framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, RBO_);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Failed to initialize MSFBO" << std::endl;
        throw std::runtime_error("Failed to initialize MSFBO");
    }

    // Also initialize the FBO/texture to blit multisampled color-buffer to;
    // used for shader operations (for postprocessing effects)
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
    this->initTexture();
    
    // Attach texture to framebuffer as its color attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId_, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
    {
        std::cerr << "Failed to initialize FBO" << std::endl;
        throw std::runtime_error("Failed to initialize FBO");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::initTexture() 
{
    glGenTextures(1, &textureId_);
    glBindTexture(GL_TEXTURE_2D, textureId_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void PostProcessor::initVAO() 
{
    GLfloat quad[] = 
    {
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
        1.0f,  1.0f, 1.0f, 1.0f,
    };

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void PostProcessor::initShaderUniforms()
{
    
    shader_->use();

    shader_->setUniform("scene", 0);

    auto offset = 1.0f / 300.0f;     // just random offset
    glm::vec2 offsets[] = 
    {
        { -offset,  offset  },  // top-left
        {  0.0f,    offset  },  // top-center
        {  offset,  offset  },  // top-right
        { -offset,  0.0f    },  // center-left
        {  0.0f,    0.0f    },  // center-center
        {  offset,  0.0f    },  // center - right
        { -offset, -offset  },  // bottom-left
        {  0.0f,   -offset  },  // bottom-center
        {  offset, -offset  }   // bottom-right
    };
    shader_->setUniform("offsets", 9, offsets);

    GLfloat blurKernel[9] = 
    {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f,  2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };
    shader_->setUniform("blurKernel", 9, blurKernel);

    shader_->end();
}

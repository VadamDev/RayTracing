#include "Framebuffer.h"

#include <glad/glad.h>

#include "../../engine/window/Window.h"

namespace engine
{
    Framebuffer::~Framebuffer()
    {
        unbind();

        glDeleteFramebuffers(1, &framebufferHandle);
        glDeleteTextures(1, &colorTextureId);
    }

    void Framebuffer::create()
    {
        //Create Framebuffer
        glGenFramebuffers(1, &framebufferHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferHandle);

        //Create color texture
        glGenTextures(1, &colorTextureId);
        glBindTexture(GL_TEXTURE_2D, colorTextureId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureId, 0);

        unbind();
    }

    void Framebuffer::resize(const int width, const int height)
    {
        this->width = width;
        this->height = height;

        glBindTexture(GL_TEXTURE_2D, colorTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Framebuffer::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferHandle);
        glViewport(0, 0, width, height);
    }

    void Framebuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, window->getWidth(), window->getHeight());
    }
}

#include "RenderingCanvas.h"

#include <glad/glad.h>

namespace editor
{
    RenderingCanvas::~RenderingCanvas()
    {
        if(textureHandle != 0)
            glDeleteTextures(1, &textureHandle);
    }

    void RenderingCanvas::create()
    {
        textureHandle = createTexture(0, GL_READ_WRITE);
    }

    void RenderingCanvas::resize(const float newWidth, const float newHeight)
    {
        this->width = newWidth;
        this->height = newHeight;

        if (textureHandle != 0)
            glDeleteTextures(1, &textureHandle);

        textureHandle = createTexture(0, GL_READ_WRITE);
    }

    unsigned int RenderingCanvas::createTexture(const unsigned int binding, const unsigned int access) const
    {
        unsigned int handle = 0;
        glCreateTextures(GL_TEXTURE_2D, 1, &handle);

        glTextureStorage2D(handle, 1, GL_RGBA32F, width, height);

        glTexParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindImageTexture(binding, handle, 0, GL_FALSE, GL_FALSE, access, GL_RGBA32F);

        return handle;
    }
}

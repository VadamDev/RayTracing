#include "Canvas.h"

#include <glad/glad.h>

namespace application
{
    Canvas::~Canvas()
    {
        if(texHandle != 0)
            glDeleteTextures(1, &texHandle);
    }

    void Canvas::create()
    {
        texHandle = createTexture();
    }

    void Canvas::resize(const int width, const int height)
    {
        this->width = width;
        this->height = height;

        if (texHandle != 0)
            glDeleteTextures(1, &texHandle);

        texHandle = createTexture();
    }

    unsigned int Canvas::createTexture() const
    {
        unsigned int texHandle = 0;
        glCreateTextures(GL_TEXTURE_2D, 1, &texHandle);

        glTextureStorage2D(texHandle, 1, GL_RGBA32F, width, height);

        glTexParameteri(texHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(texHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(texHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(texHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindImageTexture(0, texHandle, 0, GL_FALSE, GL_FALSE, GL_WRITE_ONLY, GL_RGBA32F);

        return texHandle;
    }
}

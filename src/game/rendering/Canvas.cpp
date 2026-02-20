#include "Canvas.h"

#include <glad/glad.h>

namespace application
{
    Canvas::~Canvas()
    {
        if(mainTexHandle != 0)
            glDeleteTextures(1, &mainTexHandle);
    }

    void Canvas::create()
    {
        createTextures();
    }

    void Canvas::resize(const int width, const int height)
    {
        this->width = width;
        this->height = height;

        if (mainTexHandle != 0)
            glDeleteTextures(1, &mainTexHandle);

        createTextures();
    }

    void Canvas::createTextures()
    {
        mainTexHandle = createTexture(0, GL_READ_WRITE);
    }

    unsigned int Canvas::createTexture(const unsigned int binding, const unsigned int access) const
    {
        unsigned int texHandle = 0;
        glCreateTextures(GL_TEXTURE_2D, 1, &texHandle);

        glTextureStorage2D(texHandle, 1, GL_RGBA32F, width, height);

        glTexParameteri(texHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(texHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(texHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(texHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindImageTexture(binding, texHandle, 0, GL_FALSE, GL_FALSE, access, GL_RGBA32F);

        return texHandle;
    }
}

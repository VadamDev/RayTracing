#pragma once

#include "../../engine/window/Window.h"

namespace engine
{
    class Framebuffer
    {

    public:
        Framebuffer(const int width, const int height, Window *window)
            : width(width), height(height), window(window) {}
        ~Framebuffer();

        void create();
        void resize(int width, int height);

        void bind() const;
        void unbind() const;

        int getWidth() const { return width; }
        int getHeight() const { return height; }

        unsigned int getColorTextureId() const { return colorTextureId; }

    private:
        int width, height;
        Window *window;

        unsigned int framebufferHandle = 0, colorTextureId = 0;
    };
}

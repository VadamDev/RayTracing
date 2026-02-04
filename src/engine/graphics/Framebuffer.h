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

    private:
        int width, height;
        Window *window;

        unsigned int framebufferHandle = 0;
        unsigned int colorTextureId = 0;
    };
}

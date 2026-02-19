#pragma once

#include <cstdint>

namespace application
{
    class Canvas
    {

    public:
        ~Canvas();

        void create();

        void resize(int width, int height);

        uint64_t getTextureHandle() const { return texHandle; }
        int getWidth() const { return width; }
        int getHeight() const { return height; }

    private:
        int width = 0, height = 0;
        bool bAccumulate = false;

        unsigned int texHandle = 0;

        unsigned int createTexture() const;
    };
}

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

        uint64_t getTextureHandle() const { return mainTexHandle; }
        int getWidth() const { return width; }
        int getHeight() const { return height; }
    private:
        int width = 0, height = 0;
        unsigned int mainTexHandle = 0, accumulatorTexHandle = 0;

        void createTextures();
        unsigned int createTexture(unsigned int binding, unsigned int access) const;
    };
}

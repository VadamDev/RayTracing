#pragma once

namespace editor
{
    class RenderingCanvas
    {

    public:
        RenderingCanvas(const int width, const int height)
            : width(width), height(height) {}
        ~RenderingCanvas();

        void create();
        void resize(float newWidth, float newHeight);

        unsigned int getGLTextureHandle() const { return textureHandle; }
        int getWidth() const { return width; }
        int getHeight() const { return height; }
        float getAspectRatio() const { return static_cast<float>(width) / height; }

    private:
        int width = 0, height = 0;
        unsigned int textureHandle = 0;

        unsigned int createTexture(unsigned int binding, unsigned int access) const;
    };
}

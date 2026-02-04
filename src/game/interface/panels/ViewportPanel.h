#pragma once

#include "IPanel.h"
#include "../../rendering/Renderer.h"

namespace application
{
    class ViewportPanel : public IPanel
    {

    public:
        explicit ViewportPanel(engine::Window *window, engine::Framebuffer *framebuffer)
            : IPanel("Viewport"), window(window), framebuffer(framebuffer) {}

        void draw() override;

        float aspectRatio = 16.0f / 9.0f;

    private:
        engine::Window *window;
        engine::Framebuffer *framebuffer;
    };
}

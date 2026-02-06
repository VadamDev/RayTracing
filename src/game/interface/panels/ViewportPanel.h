#pragma once

#include "IPanel.h"
#include "../../rendering/Renderer.h"

namespace application
{
    class ViewportPanel : public IPanel
    {

    public:
        explicit ViewportPanel(engine::Window *window, Renderer *renderer)
            : IPanel("Viewport"), window(window), renderer(renderer), framebuffer(renderer->getFramebuffer().get()) {}

        void draw() override;

    private:
        engine::Window *window;
        Renderer *renderer;

        engine::Framebuffer *framebuffer;
    };
}

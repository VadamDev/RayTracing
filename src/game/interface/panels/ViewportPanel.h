#pragma once

#include "UIPanel.h"
#include "../../rendering/Renderer.h"

namespace application
{
    class ViewportPanel : public UIPanel
    {

    public:
        explicit ViewportPanel(engine::Window *window, Renderer *renderer)
            : UIPanel("Viewport"), window(window), renderer(renderer), framebuffer(renderer->getFramebuffer().get()) {}

        void draw() override;

    private:
        engine::Window *window;
        Renderer *renderer;

        engine::Framebuffer *framebuffer;

        void resizeViewport(int newWidth, int newHeight) const;
        glm::ivec2 calculateViewportSize(float windowWidth, float windowHeight) const;
    };
}

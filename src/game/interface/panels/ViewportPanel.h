#pragma once

#include "UIPanel.h"
#include "../../rendering/Renderer.h"

namespace application
{
    class ViewportPanel : public UIPanel
    {

    public:
        explicit ViewportPanel(engine::Window *window, Renderer *renderer)
            : UIPanel("Viewport"), window(window), renderer(renderer), canvas(renderer->getCanvas()) {}

        void draw() override;

    private:
        engine::Window *window;
        Renderer *renderer;
        Canvas *canvas;

        glm::ivec2 calculateViewportSize(float windowWidth, float windowHeight) const;
    };
}

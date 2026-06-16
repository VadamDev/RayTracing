#pragma once

#include <glm/vec2.hpp>

#include "../UIPanel.h"

namespace engine
{
    class Window;
}

namespace editor
{
    class RenderingCanvas;

    class ViewportPanel : public UIPanel
    {

    public:
        explicit ViewportPanel(engine::Window &window, RenderingCanvas *canvas)
            : UIPanel("Viewport"), window(window), canvas(canvas) {}

        void draw(float deltaTime) override;

    private:
        engine::Window &window;
        RenderingCanvas *canvas;

        glm::ivec2 calculateViewportSize(float windowWidth, float windowHeight) const;
    };
}

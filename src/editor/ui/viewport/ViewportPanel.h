#pragma once

#include <glm/vec2.hpp>

#include "GuizmoRenderer.h"
#include "RightViewportButtonsRenderer.h"
#include "../UIPanel.h"

namespace engine
{
    class Window;
}

namespace editor
{
    class RenderingCanvas;
    class HierarchyPanel;

    class ViewportPanel : public UIPanel
    {

    public:
        explicit ViewportPanel(engine::Window &window, RenderingCanvas *canvas, HierarchyPanel *hierarchyPanel, engine::Messenger &globalMessenger, CameraSystem *cameraSystem)
            : UIPanel("Viewport"), window(window), canvas(canvas), hierarchyPanel(hierarchyPanel), guizmoRenderer(globalMessenger, cameraSystem), rightButtonsRenderer(window, globalMessenger, cameraSystem) {}

        void draw(float deltaTime) override;

    private:
        engine::Window &window;
        RenderingCanvas *canvas;
        HierarchyPanel *hierarchyPanel;

        GuizmoRenderer guizmoRenderer;
        RightViewportButtonsRenderer rightButtonsRenderer;

        glm::ivec2 calculateViewportSize(float windowWidth, float windowHeight) const;
    };
}

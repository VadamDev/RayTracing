#pragma once

#include "UIPanel.h"
#include "../../rendering/Renderer.h"
#include "ImGuizmo.h"

namespace application
{
    class HierarchyPanel;

    class ViewportPanel : public UIPanel
    {

    public:
        explicit ViewportPanel(engine::Window *window, Renderer *renderer, HierarchyPanel *hierarchyPanel)
            : UIPanel("Viewport"), window(window), renderer(renderer), canvas(renderer->getCanvas()), hierarchyPanel(hierarchyPanel) {}

        void draw() override;

    private:
        engine::Window *window;
        Renderer *renderer;
        Canvas *canvas;
        HierarchyPanel *hierarchyPanel;

        ImGuizmo::OPERATION guizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

        glm::ivec2 calculateViewportSize(float windowWidth, float windowHeight) const;
    };
}

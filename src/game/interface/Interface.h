#pragma once

#include <memory>

#include "../../engine/window/imgui/IImGuiWindow.h"
#include "../rendering/Renderer.h"
#include "panels/ViewportPanel.h"
#include "panels/HierarchyPanel.h"
#include "panels/InspectorPanel.h"

namespace application
{
    class Interface : public engine::IImGuiWindow
    {

    public:
        explicit Interface(engine::Window *window, Renderer *renderer, RaytracingApplication *application);

        void draw() override;
        bool shouldDraw() override { return true; }

    private:
        engine::Window *window;
        Renderer *renderer;

        std::unique_ptr<ViewportPanel> viewportPanel;
        std::unique_ptr<HierarchyPanel> hierarchyPanel;
        std::unique_ptr<InspectorPanel> inspectorPanel;
    };
}

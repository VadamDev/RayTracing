#include "Interface.h"

#include "imgui_internal.h"
#include "panels/ViewportPanel.h"

namespace application
{
    Interface::Interface(engine::Window *window, Renderer *renderer, RaytracingApplication *application)
        : window(window), renderer(renderer)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        viewportPanel = std::make_unique<ViewportPanel>(window, renderer->getFramebuffer().get());
        hierarchyPanel = std::make_unique<HierarchyPanel>(application);
        inspectorPanel = std::make_unique<InspectorPanel>(hierarchyPanel.get());
    }

    void Interface::draw()
    {
        static ImGuiID DOCKSPACE_ID = ImGui::GetID("My Dockspace");

        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        if (ImGui::DockBuilderGetNode(DOCKSPACE_ID) == nullptr)
        {
            ImGui::DockBuilderAddNode(DOCKSPACE_ID, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(DOCKSPACE_ID, viewport->Size);

            ImGuiID dock_id_left = 0;
            ImGuiID dock_id_main = DOCKSPACE_ID;
            ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.20f, &dock_id_left, &dock_id_main);

            ImGuiID dock_id_left_top = 0;
            ImGuiID dock_id_left_bottom = 0;
            ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.50f, &dock_id_left_top, &dock_id_left_bottom);

            ImGui::DockBuilderDockWindow(viewportPanel->getName(), dock_id_main);
            ImGui::DockBuilderDockWindow(hierarchyPanel->getName(), dock_id_left_top);
            ImGui::DockBuilderDockWindow(inspectorPanel->getName(), dock_id_left_bottom);

            ImGui::DockBuilderFinish(DOCKSPACE_ID);
        }

        ImGui::DockSpaceOverViewport(DOCKSPACE_ID, viewport, ImGuiDockNodeFlags_PassthruCentralNode);

        //Seriously I fucking hate writing UI, who the fuck is enjoying this??
        viewportPanel->draw();
        hierarchyPanel->draw();
        inspectorPanel->draw();
    }
}

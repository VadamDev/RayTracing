#include "Interface.h"

#include "imgui_internal.h"

namespace application
{
    Interface::Interface(engine::Window *window, Renderer *renderer, RaytracingApplication *application)
        : renderer(renderer)
    {
        /*
         * Config
         */

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.FontDefault = io.Fonts->AddFontFromFileTTF("resources/assets/fonts/JetBrainsMono-Regular.ttf", 18);

        /*
         * Style
         */

        ImGuiStyle &style = ImGui::GetStyle();
        style.WindowMinSize.x = 350;

        //Roundings
        style.FrameRounding = 5.0f;
        style.GrabRounding = 3.0f;
        style.ChildRounding = 3.0f;

        //Colors
        auto &colors = style.Colors;
        colors[ImGuiCol_WindowBg] = { 0.185f, 0.185f, 0.185f, 1};

        //Headers
        colors[ImGuiCol_Header] = { 0.350f, 0.350f, 0.350f, 1 };
        colors[ImGuiCol_HeaderHovered] = { 0.375f, 0.375f, 0.375f, 1 };
        colors[ImGuiCol_HeaderActive] = colors[ImGuiCol_HeaderHovered];

        //Buttons
        colors[ImGuiCol_Button] = { 0.275f, 0.275f, 0.275f, 1 };
        colors[ImGuiCol_ButtonHovered] = { 0.300f, 0.300f, 0.300f, 1 };
        colors[ImGuiCol_ButtonActive] = colors[ImGuiCol_ButtonHovered];

        //FrameBg
        colors[ImGuiCol_FrameBg] = { 0.275f, 0.275f, 0.275f, 1 };
        colors[ImGuiCol_FrameBgHovered] = { 0.325f, 0.325f, 0.325f, 1 };
        colors[ImGuiCol_FrameBgActive] = colors[ImGuiCol_FrameBgHovered];

        //Tabs
        colors[ImGuiCol_TabActive] = { 0.425f, 0.425f, 0.425f, 1 };
        colors[ImGuiCol_TabUnfocusedActive] = { 0.300f, 0.300f, 0.300f, 1};

        //Title
        colors[ImGuiCol_TitleBg] = { 0.100f, 0.100f, 0.100f, 1 };
        colors[ImGuiCol_TitleBgActive] = colors[ImGuiCol_TitleBg];

        /*
         * Create Panels
         */

        viewportPanel = std::make_unique<ViewportPanel>(window, renderer);
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

        //Seriously I fucking hate writing UI, who the fuck enjoy this??
        viewportPanel->draw();
        hierarchyPanel->draw();
        inspectorPanel->draw();
    }
}

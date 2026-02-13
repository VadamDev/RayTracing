#include "Interface.h"

#include "imgui_internal.h"
#include "panels/HierarchyPanel.h"
#include "panels/InspectorPanel.h"
#include "panels/SettingsPanel.h"
#include "panels/ViewportPanel.h"

namespace application
{
    Interface::Interface(engine::Window *window, Renderer *renderer, RaytracingApplication *application, CameraController *controller)
    {
        setupImGuiStyle();

        /*
         * Create Panels
         */

        const auto settingsPanel = registerPanel<SettingsPanel>(application, renderer, controller);
        const auto viewportPanel = registerPanel<ViewportPanel>(window, renderer);
        const auto hierarchyPanel = registerPanel<HierarchyPanel>(application);
        const auto inspectorPanel = registerPanel<InspectorPanel>(hierarchyPanel.get());
    }

    void Interface::draw()
    {
        static const ImGuiID DOCKSPACE_ID = ImGui::GetID("My Dockspace");

        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        if (ImGui::DockBuilderGetNode(DOCKSPACE_ID) == nullptr)
        {
            ImGui::DockBuilderAddNode(DOCKSPACE_ID, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(DOCKSPACE_ID, viewport->Size);

            ImGuiID dockId_Left = 0;
            ImGuiID dockId_Main = DOCKSPACE_ID;
            ImGui::DockBuilderSplitNode(dockId_Main, ImGuiDir_Left, 0.28, &dockId_Left, &dockId_Main);

            ImGuiID dockId_Settings = 0;
            ImGui::DockBuilderSplitNode(dockId_Main, ImGuiDir_Right, 0.225f, &dockId_Settings, &dockId_Main);

            ImGuiID dockId_Hierarchy = 0;
            ImGuiID dockId_Inspector = 0;
            ImGui::DockBuilderSplitNode(dockId_Left, ImGuiDir_Up, 0.50f, &dockId_Hierarchy, &dockId_Inspector);

            ImGui::DockBuilderDockWindow("Settings", dockId_Settings);
            ImGui::DockBuilderDockWindow("Viewport", dockId_Main);
            ImGui::DockBuilderDockWindow("Hierarchy", dockId_Hierarchy);
            ImGui::DockBuilderDockWindow("Inspector", dockId_Inspector);

            ImGui::DockBuilderFinish(DOCKSPACE_ID);
        }

        ImGui::DockSpaceOverViewport(DOCKSPACE_ID, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        for (const auto &panel : panels)
            panel->draw();
    }

    void Interface::setupImGuiStyle()
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
        style.WindowMinSize.x = 256;

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
    }

    template<std::derived_from<UIPanel> T, typename... Args>
    std::shared_ptr<T> Interface::registerPanel(Args&&... args)
    {
        auto panel = std::make_shared<T>(std::forward<Args>(args)...);
        panels.push_back(panel);

        return panel;
    }
}

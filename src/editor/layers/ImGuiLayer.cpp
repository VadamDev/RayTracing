#include "ImGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include "../ui/inspector/HierarchyPanel.h"
#include "../ui/inspector/InspectorPanel.h"
#include "../ui/settings/SettingsPanel.h"
#include "../ui/toolbar/ToolbarPanel.h"
#include "../ui/viewport/ViewportPanel.h"

namespace editor
{
    void ImGuiLayer::onInit(GLFWwindow *window)
    {
        // Init ImGui / ImPlot
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        // Style
        setupImGuiStyle();

        // Panels
        registerPanels();
    }

    void ImGuiLayer::registerPanels()
    {
        const auto toolbarPanel = registerPanel<ToolbarPanel>(sceneHandler);
        const auto hierarchyPanel = registerPanel<HierarchyPanel>(this->window, sceneHandler);
        const auto inspectorPanel = registerPanel<InspectorPanel>(sceneHandler, hierarchyPanel.get());
        const auto viewportPanel = registerPanel<ViewportPanel>(this->window, canvas);
        const auto settingsPanel = registerPanel<SettingsPanel>(clock, raytraceComputeLayer, canvas);
    }

    void ImGuiLayer::onFramePush(const float deltaTime)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        // Dockspace Shit
        static const ImGuiID DOCKSPACE_ID = ImGui::GetID("Dockspace");

        const ImGuiViewport *imguiViewport = ImGui::GetMainViewport();
        if (ImGui::DockBuilderGetNode(DOCKSPACE_ID) == nullptr)
        {
            ImGui::DockBuilderAddNode(DOCKSPACE_ID, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(DOCKSPACE_ID, imguiViewport->Size);

            ImGuiID dockId_Left = 0;
            ImGuiID dockId_Main = DOCKSPACE_ID;
            ImGui::DockBuilderSplitNode(dockId_Main, ImGuiDir_Left, 0.28f, &dockId_Left, &dockId_Main);

            ImGuiID dockId_Settings = 0;
            ImGui::DockBuilderSplitNode(dockId_Main, ImGuiDir_Right, 0.225f, &dockId_Settings, &dockId_Main);

            ImGuiID dockId_Hierarchy = 0;
            ImGuiID dockId_Inspector = 0;
            ImGui::DockBuilderSplitNode(dockId_Left, ImGuiDir_Up, 0.50f, &dockId_Hierarchy, &dockId_Inspector);

            ImGui::DockBuilderDockWindow("Settings", dockId_Settings);
            ImGui::DockBuilderDockWindow("Viewport", dockId_Main);
            ImGui::DockBuilderDockWindow("Hierarchy", dockId_Hierarchy);
            ImGui::DockBuilderDockWindow("Inspector", dockId_Inspector);
        }

        ImGui::DockSpaceOverViewport(DOCKSPACE_ID, imguiViewport, ImGuiDockNodeFlags_PassthruCentralNode);

        for (const auto &panel : panels)
            panel->draw(deltaTime);
    }

    void ImGuiLayer::onFramePop()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::onDestroy() noexcept
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::setupImGuiStyle()
    {
        /*
         * ImGui IO
         */

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.FontDefault = io.Fonts->AddFontFromFileTTF("resources/fonts/JetBrainsMono-Regular.ttf", 18);

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
    std::shared_ptr<T> ImGuiLayer::registerPanel(Args&&... args)
    {
        auto panel = std::make_shared<T>(std::forward<Args>(args)...);
        panels.push_back(panel);

        return panel;
    }
}

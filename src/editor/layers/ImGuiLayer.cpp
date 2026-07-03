#include "ImGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>
#include <IconsFontAwesome7.h>

#include "../../engine/messenger/Messenger.hpp"
#include "../rendering/RenderManager.h"
#include "../ui/editor/inspector/HierarchyPanel.h"
#include "../ui/editor/inspector/InspectorPanel.h"
#include "../ui/editor/settings/RenderSettingsPanel.h"
#include "../ui/editor/settings/SettingsPanel.h"
#include "../ui/editor/toolbar/ToolbarPanel.h"
#include "../ui/editor/viewport/ViewportPanel.h"
#include "../ui/preview/RenderPreviewPanel.h"
#include "../ui/preview/RenderStatusPanel.h"

namespace editor
{
    void ImGuiLayer::onInit(GLFWwindow *window)
    {
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        setupImGuiStyle();
        registerPanels();
        registerListeners();
    }

    void ImGuiLayer::registerPanels()
    {
        const auto renderSettingsMenu = registerPanel<RenderSettingsPanel>(PanelType::MENU, globalMessenger, canvas);

        // Editor Panels
        registerPanel<ToolbarPanel>(PanelType::EDITOR, globalMessenger, sceneHandler, renderManager, renderSettingsMenu);
        const auto hierarchyPanel = registerPanel<HierarchyPanel>(PanelType::EDITOR, this->window, sceneHandler);
        registerPanel<InspectorPanel>(PanelType::EDITOR, sceneHandler, hierarchyPanel);
        registerPanel<ViewportPanel>(PanelType::EDITOR, this->window, canvas, hierarchyPanel, globalMessenger, cameraSystem);
        registerPanel<SettingsPanel>(PanelType::EDITOR, window, clock, raytraceComputeLayer, canvas);

        // Render Only Panels
        registerPanel<RenderPreviewPanel>(PanelType::PREVIEW, canvas);
        registerPanel<RenderStatusPanel>(PanelType::PREVIEW, clock, renderManager);
    }

    void ImGuiLayer::registerListeners()
    {
        globalMessenger.subscribe<OpenMenuEvent>([this](const OpenMenuEvent *event) {
            openMenu(event->name);
        });

        globalMessenger.subscribe<CloseMenuEvent>([this](const CloseMenuEvent *event) {
            closeMenu(event->name);
        });
    }

    void ImGuiLayer::onFramePush(const float deltaTime)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        switch (renderManager->getRenderMode())
        {
            case RenderMode::EDITOR:
                drawEditor(deltaTime);
                break;
            case RenderMode::RENDER_ONLY:
                drawPreview(deltaTime);
                break;
        }
    }

    void ImGuiLayer::drawEditor(const float deltaTime) const
    {
        ImGuizmo::BeginFrame();

        // Dockspace Init
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

        // Draw
        ImGui::DockSpaceOverViewport(DOCKSPACE_ID, imguiViewport, ImGuiDockNodeFlags_PassthruCentralNode);

        const bool isMenuOpened = !openedMenus.empty();
        if (isMenuOpened)
        {
            for (const auto &menu : openedMenus)
                menu->draw(deltaTime);

            ImGui::BeginDisabled();
        }

        for (const auto &panel : getPanels(PanelType::EDITOR))
            panel->draw(deltaTime);

        if (isMenuOpened)
            ImGui::EndDisabled();
    }

    void ImGuiLayer::drawPreview(const float deltaTime) const
    {
        for (const auto &panel : getPanels(PanelType::PREVIEW))
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

    void ImGuiLayer::openMenu(const std::string_view name)
    {
        UIPanel *menu = findMenu(name);
        if (std::ranges::count(openedMenus, menu) > 0)
            return;

        openedMenus.push_back(menu);
    }

    void ImGuiLayer::closeMenu(const std::string_view name)
    {
        UIPanel *menu = findMenu(name);
        if (std::ranges::count(openedMenus, menu) <= 0)
            return;

        std::erase(openedMenus, menu);
    }

    UIPanel *ImGuiLayer::findMenu(const std::string_view name)
    {
        const auto &it = menuDictionary.find(name.data());
        if (it == menuDictionary.end())
            throw std::runtime_error(std::format("Failed to find menu with name: {}", name));

        return it->second;
    }

    void ImGuiLayer::setupImGuiStyle()
    {
        /*
         * ImGui IO
         */

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        registerFonts();

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

    void ImGuiLayer::registerFonts()
    {
        static constexpr float BASE_FONT_SIZE = 18.0f;
        static constexpr float ICON_FONT_SIZE = BASE_FONT_SIZE * (2.0f / 3.0f);

        ImGuiIO &io = ImGui::GetIO();
        io.FontDefault = io.Fonts->AddFontFromFileTTF("resources/fonts/JetBrainsMono-Regular.ttf", BASE_FONT_SIZE);

        ImFontConfig config;
        config.MergeMode = true;
        config.PixelSnapH = true;
        config.GlyphMinAdvanceX = ICON_FONT_SIZE;

        static const ImWchar FONT_AWESOME_RANGES[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };

        io.Fonts->AddFontFromFileTTF("resources/fonts/FontAwesome7_Free-Solid-900.otf", ICON_FONT_SIZE, &config, FONT_AWESOME_RANGES);
        io.Fonts->Build();
    }

    template<std::derived_from<UIPanel> T, typename... Args>
    T* ImGuiLayer::registerPanel(PanelType type, Args&&... args)
    {
        auto panel = std::make_unique<T>(std::forward<Args>(args)...);
        T* panelPtr = panel.get();

        if (type == PanelType::MENU)
            menuDictionary.emplace(panel->getName(), panelPtr);

        panels[static_cast<int>(type)].push_back(std::move(panel));
        return panelPtr;
    }

    std::vector<UIPanel*> ImGuiLayer::getPanels(PanelType type) const
    {
        std::vector<UIPanel*> result;

        const auto &subPanelList = panels[static_cast<int>(type)];
        result.reserve(subPanelList.size());
        for (const auto &panel : subPanelList)
            result.push_back(panel.get());

        return result;
    }
}

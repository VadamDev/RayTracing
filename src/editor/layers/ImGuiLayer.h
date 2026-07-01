#pragma once

#include <vector>
#include <memory>

#include "RaytraceComputeLayer.h"
#include "../../engine/window/IRenderLayer.h"
#include "../ui/UIPanel.h"

namespace engine
{
    class SimpleClock;
    class Window;
}

namespace editor
{
    class SceneHandler;
    class RenderingCanvas;
    class RenderManager;
    class RaytraceComputeLayer;

    static constexpr int NUM_PANEL_TYPES = 3;
    enum class PanelType : int
    {
        EDITOR = 0,
        PREVIEW = 1,
        MENU = 2
    };

    class ImGuiLayer : public engine::IRenderLayer
    {

    public:
        explicit ImGuiLayer(engine::Window &window, engine::SimpleClock *clock, engine::Messenger &globalMessenger, RenderManager *renderManager, RaytraceComputeLayer *raytraceComputeLayer, SceneHandler *sceneHandler, RenderingCanvas *canvas, CameraSystem *cameraSystem)
            : window(window), clock(clock), globalMessenger(globalMessenger), renderManager(renderManager), raytraceComputeLayer(raytraceComputeLayer), sceneHandler(sceneHandler), canvas(canvas), cameraSystem(cameraSystem) {}

        void onInit(GLFWwindow *window) override;

        bool canRender() const override { return true; }
        void onFramePush(float deltaTime) override;
        void onFramePop() override;

        void onDestroy() noexcept override;
    private:
        engine::Window &window;
        engine::SimpleClock *clock;
        engine::Messenger &globalMessenger;

        RaytraceComputeLayer *raytraceComputeLayer;

        SceneHandler *sceneHandler;
        RenderingCanvas *canvas;
        CameraSystem *cameraSystem;
        RenderManager *renderManager;

        std::array<std::vector<std::unique_ptr<UIPanel>>, NUM_PANEL_TYPES> panels;
        std::unordered_map<std::string, UIPanel*> menuDictionary;
        std::vector<UIPanel*> openedMenus;

        void registerPanels();
        void registerListeners();

        void drawEditor(float deltaTime) const;
        void drawPreview(float deltaTime) const;

        template<std::derived_from<UIPanel> T, typename... Args>
        T* registerPanel(PanelType type, Args&&... args);

        std::vector<UIPanel*> getPanels(PanelType type) const;

        void openMenu(std::string_view name);
        void closeMenu(std::string_view name);
        UIPanel* findMenu(std::string_view name);

        static void setupImGuiStyle();
        static void registerFonts();
    };
}

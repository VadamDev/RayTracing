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

    class ImGuiLayer : public engine::IRenderLayer
    {

    public:
        explicit ImGuiLayer(engine::Window &window, engine::SimpleClock *clock, RenderManager *renderManager, RaytraceComputeLayer *raytraceComputeLayer, SceneHandler *sceneHandler, RenderingCanvas *canvas, CameraSystem *cameraSystem)
            : window(window), clock(clock), renderManager(renderManager), raytraceComputeLayer(raytraceComputeLayer), sceneHandler(sceneHandler), canvas(canvas), cameraSystem(cameraSystem) {}

        void onInit(GLFWwindow *window) override;

        bool canRender() const override { return true; }
        void onFramePush(float deltaTime) override;
        void onFramePop() override;

        void onDestroy() noexcept override;

        engine::Messenger& getGlobalMessenger() const { return raytraceComputeLayer->getGlobalMessenger(); }
    private:
        engine::Window &window;
        engine::SimpleClock *clock;

        RaytraceComputeLayer *raytraceComputeLayer;

        SceneHandler *sceneHandler;
        RenderingCanvas *canvas;
        CameraSystem *cameraSystem;
        RenderManager *renderManager;

        std::vector<std::shared_ptr<UIPanel>> editorPanels, renderOnlyPanels;

        void registerPanels();

        void drawEditor(float deltaTime) const;
        void drawPreview(float deltaTime) const;

        template<std::derived_from<UIPanel> T, typename... Args>
        std::shared_ptr<T> registerEditorPanel(Args&&... args);

        template<std::derived_from<UIPanel> T, typename... Args>
        std::shared_ptr<T> registerPreviewPanel(Args&&... args);

        template<std::derived_from<UIPanel> T, typename... Args>
        static std::shared_ptr<T> registerPanel(std::vector<std::shared_ptr<UIPanel>> &panels, Args&&... args);

        static void setupImGuiStyle();
        static void registerFonts();
    };
}

#pragma once

#include "../../engine/clock/SimpleClock.h"
#include "../../engine/window/IRenderLayer.h"
#include "../ui/UIPanel.h"

namespace editor
{
    class SceneHandler;
    class RenderingCanvas;

    class ImGuiLayer : public engine::IRenderLayer
    {

    public:
        explicit ImGuiLayer(engine::Window &window, SceneHandler *sceneHandler, RenderingCanvas *canvas)
            : window(window), sceneHandler(sceneHandler), canvas(canvas) {}

        void onInit(GLFWwindow *window) override;

        bool canRender() const override { return true; }
        void onFramePush(float deltaTime) override;
        void onFramePop() override;

        void onDestroy() noexcept override;

    private:
        engine::Window &window;
        SceneHandler *sceneHandler;
        RenderingCanvas *canvas;

        std::vector<std::shared_ptr<UIPanel>> panels;

        template<std::derived_from<UIPanel> T, typename... Args>
        std::shared_ptr<T> registerPanel(Args&&... args);

        static void setupImGuiStyle();
    };
}

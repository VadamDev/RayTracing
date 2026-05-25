#pragma once

#include "../../engine/clock/SimpleClock.h"
#include "../../engine/window/IRenderLayer.h"
#include "../ui/UIPanel.h"

namespace editor
{
    class SceneHandler;

    class ImGuiLayer : public engine::IRenderLayer
    {

    public:
        explicit ImGuiLayer(engine::SimpleClock *clock, SceneHandler *sceneHandler)
            : clock(clock), sceneHandler(sceneHandler) {}

        void onInit(GLFWwindow *window) override;

        bool canRender() const override { return true; }
        void onFramePush(float deltaTime) const override;
        void onFramePop() const override;

        void onDestroy() const noexcept override;

    private:
        engine::SimpleClock *clock;
        SceneHandler *sceneHandler;

        std::vector<std::shared_ptr<UIPanel>> panels;

        template<std::derived_from<UIPanel> T, typename... Args>
        std::shared_ptr<T> registerPanel(Args&&... args);

        static void setupImGuiStyle();
    };
}

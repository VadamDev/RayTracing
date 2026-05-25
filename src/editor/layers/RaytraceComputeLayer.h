#pragma once

#include "../../engine/clock/SimpleClock.h"
#include "../../engine/window/IRenderLayer.h"

namespace editor
{
    class SceneHandler;

    class RaytraceComputeLayer : public engine::IRenderLayer
    {

    public:
        explicit RaytraceComputeLayer(SceneHandler *sceneHandler)
            : sceneHandler(sceneHandler) {}

        void onInit(GLFWwindow *window) override;

        bool canRender() const override { return true; }

        void onFramePush(float deltaTime) const override;
        void onFramePop() const override;

        void onDestroy() const noexcept override;

    private:
        SceneHandler *sceneHandler;
    };
}

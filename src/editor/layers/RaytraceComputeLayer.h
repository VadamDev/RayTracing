#pragma once

#include "../../engine/clock/SimpleClock.h"
#include "../../engine/window/IRenderLayer.h"

namespace editor
{
    class RaytraceComputeLayer : public engine::IRenderLayer
    {

    public:
        explicit RaytraceComputeLayer(engine::SimpleClock *clock)
            : clock(clock) {}

        void onInit(GLFWwindow *window) override;

        bool canRender() const override { return true; }

        void onFramePush(float deltaTime) const override;
        void onFramePop() const override;

        void onDestroy() const noexcept override;

    private:
        engine::SimpleClock *clock;
    };
}

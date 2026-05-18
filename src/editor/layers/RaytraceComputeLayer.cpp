#include "RaytraceComputeLayer.h"

#include <spdlog/spdlog.h>

namespace editor
{
    void RaytraceComputeLayer::onInit(GLFWwindow *window)
    {

    }

    void RaytraceComputeLayer::onFramePush(const float deltaTime) const
    {
        static float timer = -1;
        timer += deltaTime;

        if (timer >= 1)
        {
            spdlog::info("FPS: {} ({} ms)", clock->getFPS(), clock->getRenderProfiler()->getLastSpentTimeMs());
            timer = 0;
        }
    }

    void RaytraceComputeLayer::onFramePop() const
    {

    }

    void RaytraceComputeLayer::onDestroy() const noexcept
    {

    }
}

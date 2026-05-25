#include "RaytraceComputeLayer.h"

#include "../scene/SceneHandler.h"

namespace editor
{
    void RaytraceComputeLayer::onInit(GLFWwindow *window)
    {

    }

    void RaytraceComputeLayer::onFramePush(const float deltaTime) const
    {
        sceneHandler->onFramePush();
    }

    void RaytraceComputeLayer::onFramePop() const
    {

    }

    void RaytraceComputeLayer::onDestroy() const noexcept
    {

    }
}

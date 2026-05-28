#include "RaytraceComputeLayer.h"

#include "../rendering/RenderingCanvas.h"
#include "../scene/SceneHandler.h"

namespace editor
{
    void RaytraceComputeLayer::onInit(GLFWwindow *window)
    {
        shader.create();
    }

    void RaytraceComputeLayer::onFramePush(const float deltaTime) {
        sceneHandler->onFramePush();
        cameraSystem->findPrimaryCamera();

        shader.bind();

        shader.updateFrameIndex(frameIndex++);
        shader.updateViewData(cameraSystem, canvas->getAspectRatio());
        shader.updateRaytracerSettings(settings);
        shader.updateRaytracerDebugSettings(debugSettings);

        RaytraceShader::dispatchCompute(ceil(canvas->getWidth() / 8), ceil(canvas->getHeight() / 8), 1, GL_TEXTURE_FETCH_BARRIER_BIT);

        shader.unbindBuffers();
        shader.unbind();
    }

    void RaytraceComputeLayer::onFramePop()
    {

    }

    void RaytraceComputeLayer::onDestroy() noexcept
    {

    }
}

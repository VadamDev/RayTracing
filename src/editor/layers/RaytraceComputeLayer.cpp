#include "RaytraceComputeLayer.h"

#include "../rendering/RenderingCanvas.h"
#include "../scene/SceneHandler.h"

namespace editor
{
    void RaytraceComputeLayer::onInit(GLFWwindow *window)
    {
        shader.create();

        sphereCpSystem = std::make_unique<RaytracedSphereSystem>(shader);
        meshCpSystem = std::make_unique<RaytracedMeshSystem>(shader, modelManager);
    }

    bool RaytraceComputeLayer::canRender() const
    {
        return sceneHandler->isSceneOpened();
    }

    void RaytraceComputeLayer::onFramePush(const float deltaTime) {
        engine::Scene *openedScene = sceneHandler->getOpenedScene();

        shader.bind();

        shader.updateMeshDataBuffers(modelManager->getAllTriangles(), modelManager->getAllBvhNodes());
        sphereCpSystem->updateData(openedScene);
        meshCpSystem->updateData(openedScene);

        shader.updateFrameIndex(frameIndex++);
        shader.updateViewData(cameraSystem, canvas->getAspectRatio());
        shader.updateRaytracerSettings(settings);
        shader.updateRaytracerDebugSettings(debugSettings);

        RaytraceShader::dispatchCompute(ceil(canvas->getWidth() / 8), ceil(canvas->getHeight() / 8), 1, GL_TEXTURE_FETCH_BARRIER_BIT);

        shader.unbindBuffers();
        shader.unbind();
    }

    void RaytraceComputeLayer::onDestroy() noexcept
    {

    }
}

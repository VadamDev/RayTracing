#include "RaytraceComputeLayer.h"

#include "../../engine/messenger/Messenger.hpp"
#include "../rendering/RenderingCanvas.h"
#include "../scene/SceneHandler.h"
#include "../rendering/RenderingEvents.h"

namespace editor
{
    static constexpr int N_GROUP_X = 8;
    static constexpr int N_GROUP_Y = 8;

    void RaytraceComputeLayer::onInit(GLFWwindow *window)
    {
        shader.create();

        sphereCpSystem = std::make_unique<RaytracedSphereSystem>(shader);
        meshCpSystem = std::make_unique<RaytracedMeshSystem>(shader, modelManager);

        registerListeners();
    }

    void RaytraceComputeLayer::registerListeners()
    {
        // Reset accumulation on camera move
        globalMessenger.subscribe<PrimaryCameraMovedEvent>([this](const PrimaryCameraMovedEvent *event) {
            dispatchResetAccumulationEvent();
        });

        // Reset frame accumulation
        globalMessenger.subscribe<AccumulationResetEvent>([this](const AccumulationResetEvent *event) {
            frameIndex = event->newFrameIndex;
        });

        // Add / Remove entity to a scene update the buffers
        globalMessenger.subscribe<engine::EntityAddedToSceneEvent>([this](const engine::EntityAddedToSceneEvent *event) {
            dispatchUpdateBuffersEvent();
        });

        globalMessenger.subscribe<engine::EntityRemovedFromSceneEvent>([this](const engine::EntityRemovedFromSceneEvent *event) {
            dispatchUpdateBuffersEvent();
        });

        // Whole scene is sent to the GPU when this event is fired
        globalMessenger.subscribe<UpdateRaytracedObjectsBuffersEvent>([this](const UpdateRaytracedObjectsBuffersEvent *event) {
            if (event->resetAccumulation)
                dispatchResetAccumulationEvent();

            shouldUpdateRaytracedObjectBuffers = true;
        });

        globalMessenger.subscribe<UpdateMeshesDataBuffersEvent>([this](const UpdateMeshesDataBuffersEvent *event) {
            if (event->resetAccumulation)
                dispatchResetAccumulationEvent();

            shouldUpdateMeshesDataBuffers = true;
        });
    }

    bool RaytraceComputeLayer::canRender() const
    {
        return sceneHandler->isSceneOpened();
    }

    void RaytraceComputeLayer::onFramePush(const float deltaTime) {
        engine::Scene *openedScene = sceneHandler->getOpenedScene();

        shader.bind();

        updateBuffers(openedScene);
        shader.updateFrameIndex(frameIndex++);
        shader.updateViewData(cameraSystem, canvas->getAspectRatio());
        shader.updateRaytracerSettings(settings);
        shader.updateRaytracerDebugSettings(debugSettings);

        RaytraceShader::dispatchCompute(ceil(canvas->getWidth() / N_GROUP_X), ceil(canvas->getHeight() / N_GROUP_Y), 1, GL_TEXTURE_FETCH_BARRIER_BIT);

        shader.unbindBuffers();
        shader.unbind();
    }

    void RaytraceComputeLayer::updateBuffers(engine::Scene *openedScene)
    {
        if (shouldUpdateRaytracedObjectBuffers)
        {
            sphereCpSystem->updateData(openedScene);
            meshCpSystem->updateData(openedScene);

            shouldUpdateRaytracedObjectBuffers = false;
        }

        if (shouldUpdateMeshesDataBuffers)
        {
            shader.updateMeshDataBuffers(modelManager->getAllTriangles(), modelManager->getAllBvhNodes());
            shouldUpdateMeshesDataBuffers = false;
        }
    }

    void RaytraceComputeLayer::dispatchResetAccumulationEvent() const
    {
        AccumulationResetEvent event;
        globalMessenger.dispatch(event);
    }

    void RaytraceComputeLayer::dispatchUpdateBuffersEvent() const
    {
        UpdateRaytracedObjectsBuffersEvent event;
        globalMessenger.dispatch(event);
    }
}

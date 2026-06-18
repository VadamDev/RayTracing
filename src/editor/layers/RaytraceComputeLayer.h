#pragma once

#include "../../engine/window/IRenderLayer.h"
#include "../rendering/RaytracedObjectsSystem.h"
#include "../rendering/RaytraceShader.h"

namespace editor
{
    class SceneHandler;
    class ModelManager;
    class RenderingCanvas;
    class CameraSystem;

    class RaytraceComputeLayer : public engine::IRenderLayer
    {

    public:
        explicit RaytraceComputeLayer(engine::Messenger &globalMessenger, SceneHandler *sceneHandler, ModelManager *modelManager, RenderingCanvas *canvas, CameraSystem *cameraSystem)
            : globalMessenger(globalMessenger), sceneHandler(sceneHandler), modelManager(modelManager), canvas(canvas), cameraSystem(cameraSystem) {}

        void onInit(GLFWwindow *window) override;

        bool canRender() const override;

        void onFramePush(float deltaTime) override;
        void onFramePop() override {}

        void onDestroy() noexcept override {}

        engine::Messenger& getGlobalMessenger() const { return globalMessenger; }

        RaytracerSettings settings;
        DebugSettings debugSettings;
    private:
        engine::Messenger &globalMessenger;

        SceneHandler *sceneHandler;
        ModelManager *modelManager;

        RenderingCanvas *canvas;
        CameraSystem *cameraSystem;

        RaytraceShader shader;
        std::unique_ptr<RaytracedSphereSystem> sphereCpSystem;
        std::unique_ptr<RaytracedMeshSystem> meshCpSystem;

        unsigned int frameIndex = 1;

        void registerListeners();

        bool shouldUpdateRaytracedObjectBuffers = true, shouldUpdateMeshesDataBuffers = true;
        void updateBuffers(engine::Scene *openedScene);
        void dispatchResetAccumulationEvent() const;
        void dispatchUpdateBuffersEvent() const;
    };
}

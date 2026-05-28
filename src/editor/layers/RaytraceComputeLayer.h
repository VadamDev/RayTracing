#pragma once

#include "../../engine/window/IRenderLayer.h"
#include "../rendering/RaytraceShader.h"

namespace editor
{
    class SceneHandler;
    class RenderingCanvas;
    class CameraSystem;

    class RaytraceComputeLayer : public engine::IRenderLayer
    {

    public:
        explicit RaytraceComputeLayer(SceneHandler *sceneHandler, RenderingCanvas *canvas, CameraSystem *cameraSystem)
            : sceneHandler(sceneHandler), canvas(canvas), cameraSystem(cameraSystem) {}

        void onInit(GLFWwindow *window) override;

        bool canRender() const override { return true; }

        void onFramePush(float deltaTime) override;
        void onFramePop() override;

        void onDestroy() noexcept override;

        RaytracerSettings settings;
        DebugSettings debugSettings;
    private:
        SceneHandler *sceneHandler;

        RenderingCanvas *canvas;
        CameraSystem *cameraSystem;

        RaytraceShader shader;

        unsigned int frameIndex = 1;
    };
}

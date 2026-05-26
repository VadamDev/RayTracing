#pragma once

#include "../engine/Application.h"
#include "rendering/RenderingCanvas.h"
#include "rendering/CameraSystem.h"

namespace editor
{
    class SceneHandler;

    class RenderingCanvas;

    class RaytracingApp : public engine::Application
    {

    public:
        using Application::Application;

        void init() override;
        void processInputs(float deltaTime) override;
        void destroy() noexcept override;

    private:
        std::unique_ptr<SceneHandler> sceneHandler;

        std::unique_ptr<RenderingCanvas> canvas;
        std::unique_ptr<CameraSystem> cameraSystem;
    };
}

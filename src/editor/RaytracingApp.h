#pragma once

#include "../engine/Application.h"
#include "model/ModelManager.h"
#include "scene/SceneHandler.h"
#include "rendering/RenderingCanvas.h"
#include "rendering/CameraSystem.h"

namespace editor
{
    class RaytracingApp : public engine::Application
    {

    public:
        using Application::Application;

        void onInit() override;
        void onPostInit() override;
        void onPreRender(float deltaTime) override;
        void onDestroy() noexcept override;

    private:
        std::unique_ptr<SceneHandler> sceneHandler;

        std::unique_ptr<ModelManager> modelManager;

        std::unique_ptr<RenderingCanvas> canvas;
        std::unique_ptr<CameraSystem> cameraSystem;
    };
}

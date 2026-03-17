#pragma once

#include "../engine/Application.h"
#include "../engine/scene/Scene.h"
#include "controller/CameraController.h"
#include "interface/Interface.h"
#include "model/ModelManager.h"
#include "rendering/Renderer.h"

namespace application
{
    class RaytracingApplication : public engine::Application
    {

    public:
        using Application::Application;

        void init() override;

        void update() override;
        void processInputs(float deltaTime) override;
        void render(float deltaTime) override;

        void destroy() noexcept override;

        engine::Scene& getActiveScene() { return scene; }
        ModelManager& getModelManager() { return modelManager; }

    private:
        ModelManager modelManager;

        engine::Scene scene;

        Renderer renderer;
        std::unique_ptr<CameraController> controller;

        std::shared_ptr<Interface> interface;
    };
}

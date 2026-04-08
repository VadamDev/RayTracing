#pragma once

#include "../engine/Application.h"
#include "../engine/scene/Scene.h"
#include "../engine/scene/SceneSerializer.h"
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

        ModelManager& getModelManager() { return modelManager; }

        engine::SceneSerializer& getSceneSerializer() { return sceneSerializer; }
        engine::Scene& getActiveScene() { return scene; }
        void setActiveScene(engine::Scene &scene) { this->scene = std::move(scene); }

    private:
        ModelManager modelManager;

        engine::SceneSerializer sceneSerializer;
        engine::Scene scene;

        Renderer renderer;
        std::unique_ptr<CameraController> controller;

        std::shared_ptr<Interface> interface;
    };
}

#include "RaytracingApp.h"

#include "controller/FreecamController.h"
#include "layers/ImGuiLayer.h"
#include "layers/RaytraceComputeLayer.h"
#include "../engine/clock/SimpleClock.h"

namespace editor
{
    void RaytracingApp::onInit()
    {
        auto clock = dynamic_cast<engine::SimpleClock*>(this->clock);

        sceneHandler = std::make_unique<SceneHandler>(globalMessenger);
        sceneHandler->openScene("resources/scenes/default_scene.json");

        modelManager = std::make_unique<ModelManager>(globalMessenger);

        canvas = std::make_unique<RenderingCanvas>(window.getWidth(), window.getHeight());

        cameraSystem = std::make_unique<CameraSystem>(globalMessenger, canvas.get(), sceneHandler.get());
        cameraSystem->registerController<FreecamController>(cameraSystem.get(), window);

        renderManager = std::make_unique<RenderManager>(globalMessenger);

        const auto raytraceLayer = window.registerLayer<RaytraceComputeLayer>(globalMessenger, sceneHandler.get(), modelManager.get(), canvas.get(), cameraSystem.get());
        const auto imguiLayer = window.registerLayer<ImGuiLayer>(window, clock, renderManager.get(), raytraceLayer.get(), sceneHandler.get(), canvas.get(), cameraSystem.get());
    }

    void RaytracingApp::onPostInit()
    {
        canvas->create();
    }

    void RaytracingApp::onPreRender(const float deltaTime)
    {
        if (renderManager->getRenderMode() == RenderMode::RENDER_ONLY)
        {
            renderManager->onFramePush();
            return;
        }

        if (window.getInputsManager().isKeyDown(engine::KeyboardKeys::KEY_ESCAPE) && window.isGrabbed())
            window.setGrabbed(false);

        sceneHandler->onFramePush();

        cameraSystem->findPrimaryCamera(); // TODO: update only when necessary
        cameraSystem->processInputs(deltaTime);
    }
}

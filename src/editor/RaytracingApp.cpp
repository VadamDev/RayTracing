#include "RaytracingApp.h"

#include "controller/FreecamController.h"
#include "layers/ImGuiLayer.h"
#include "layers/RaytraceComputeLayer.h"

namespace editor
{
    void RaytracingApp::onInit()
    {
        sceneHandler = std::make_unique<SceneHandler>(&globalMessenger);
        sceneHandler->openNewEmptyScene();

        modelManager = std::make_unique<ModelManager>();

        canvas = std::make_unique<RenderingCanvas>(window.getWidth(), window.getHeight());

        cameraSystem = std::make_unique<CameraSystem>(canvas.get(), sceneHandler.get());
        cameraSystem->registerController<FreecamController>(cameraSystem.get(), window.getInputsManager());

        window.registerLayer<RaytraceComputeLayer>(sceneHandler.get(), modelManager.get(), canvas.get(), cameraSystem.get());
        window.registerLayer<ImGuiLayer>(window, sceneHandler.get(), canvas.get());
    }

    void RaytracingApp::onPostInit()
    {
        canvas->create();
    }

    void RaytracingApp::onPreRender(const float deltaTime)
    {
        if (window.getInputsManager().isKeyDown(engine::KeyboardKeys::KEY_ESCAPE) && window.isGrabbed())
            window.setGrabbed(false);

        sceneHandler->onFramePush();

        cameraSystem->findPrimaryCamera(); // TODO: update only when necessary
        cameraSystem->processInputs(deltaTime);
    }

    void RaytracingApp::onDestroy() noexcept
    {

    }
}

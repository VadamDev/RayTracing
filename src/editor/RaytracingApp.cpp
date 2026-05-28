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

        canvas = std::make_unique<RenderingCanvas>(window.getWidth(), window.getHeight());

        cameraSystem = std::make_unique<CameraSystem>(canvas.get(), sceneHandler.get());
        cameraSystem->registerController<FreecamController>(cameraSystem.get(), window.getInputsManager());

        window.registerLayer<RaytraceComputeLayer>(sceneHandler.get(), canvas.get(), cameraSystem.get());
        window.registerLayer<ImGuiLayer>(window, sceneHandler.get(), canvas.get());
    }

    void RaytracingApp::onPostInit()
    {
        canvas->create();
    }

    void RaytracingApp::onProcessInputs(const float deltaTime)
    {
        if (window.getInputsManager().isKeyDown(engine::KeyboardKeys::KEY_ESCAPE) && window.isGrabbed())
            window.setGrabbed(false);

        cameraSystem->processInputs(deltaTime);
    }

    void RaytracingApp::onDestroy() noexcept
    {

    }
}

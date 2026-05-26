#include "RaytracingApp.h"

#include "scene/SceneHandler.h"
#include "controller/FreecamController.h"
#include "layers/ImGuiLayer.h"
#include "layers/RaytraceComputeLayer.h"

namespace editor
{
    void RaytracingApp::init()
    {
        sceneHandler = std::make_unique<SceneHandler>(&globalMessenger);
        sceneHandler->openNewEmptyScene();

        canvas = std::make_unique<RenderingCanvas>(window.getWidth(), window.getHeight());
        cameraSystem = std::make_unique<CameraSystem>(canvas.get(), sceneHandler.get());
        cameraSystem->registerController<FreecamController>(cameraSystem.get(), window.getInputsManager());

        auto clock = dynamic_cast<engine::SimpleClock*>(this->clock);
        window.registerLayer<RaytraceComputeLayer>(sceneHandler.get());
        window.registerLayer<ImGuiLayer>(clock, sceneHandler.get());
    }

    void RaytracingApp::processInputs(const float deltaTime)
    {
        cameraSystem->processInputs(deltaTime);
    }

    void RaytracingApp::destroy() noexcept
    {

    }
}

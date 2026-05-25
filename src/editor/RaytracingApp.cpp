#include "RaytracingApp.h"

#include "layers/ImGuiLayer.h"
#include "layers/RaytraceComputeLayer.h"

namespace editor
{
    void RaytracingApp::init()
    {
        sceneHandler = std::make_unique<SceneHandler>(&globalMessenger);
        sceneHandler->openNewEmptyScene();

        auto clock = dynamic_cast<engine::SimpleClock*>(this->clock);

        window.registerLayer<RaytraceComputeLayer>(sceneHandler.get());
        window.registerLayer<ImGuiLayer>(clock, sceneHandler.get());
    }

    void RaytracingApp::processInputs(const float deltaTime)
    {

    }

    void RaytracingApp::destroy() noexcept
    {

    }
}

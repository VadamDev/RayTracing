#include "RaytracingApp.h"

#include "layers/ImGuiLayer.h"
#include "layers/RaytraceComputeLayer.h"

namespace editor
{
    void RaytracingApp::init()
    {
        auto clock = dynamic_cast<engine::SimpleClock*>(this->clock);

        window.registerLayer<RaytraceComputeLayer>(clock);
        window.registerLayer<ImGuiLayer>(clock);
    }

    void RaytracingApp::processInputs(const float deltaTime)
    {

    }

    void RaytracingApp::destroy() noexcept
    {

    }
}

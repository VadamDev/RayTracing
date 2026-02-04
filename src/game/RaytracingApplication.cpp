#include "RaytracingApplication.h"

namespace application
{
    void RaytracingApplication::init()
    {
        renderer.init(window);
        controller = std::make_unique<CameraController>(renderer.getCamera(), window.getInputsManager());

        interface = std::make_shared<Interface>();
        window.registerImGuiWindow(interface);
    }

    void RaytracingApplication::update()
    {

    }

    void RaytracingApplication::processInputs(const float deltaTime)
    {
        const auto inputsManager = window.getInputsManager();
        if (inputsManager->isMouseButtonDown(engine::MouseButton::BUTTON_1) && !window.isGrabbed() && !window.wantCapturePeripherals())
            window.setGrabbed(true);
        else if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_ESCAPE) && window.isGrabbed())
            window.setGrabbed(false);

        if (window.isGrabbed())
        {
            controller->processInputs(deltaTime);
        }
    }

    void RaytracingApplication::render(const float deltaTime)
    {
        renderer.render();
    }

    void RaytracingApplication::destroy() noexcept
    {

    }
}

#include "RaytracingApplication.h"

#include "DefaultScene.h"

namespace application
{
    void RaytracingApplication::init()
    {
        renderer.init(window, this);
        controller = std::make_unique<CameraController>(renderer.getCamera(), window.getInputsManager());

        interface = std::make_shared<Interface>(&window, &renderer, this, controller.get());
        window.registerImGuiWindow(interface);

        setupDefaultScene(scene);
    }

    void RaytracingApplication::update()
    {

    }

    void RaytracingApplication::processInputs(const float deltaTime)
    {
        if (!window.isGrabbed())
            return;

        const auto inputsManager = window.getInputsManager();

        //Ungrab Viewport
        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_ESCAPE))
            window.setGrabbed(false);

        //Camera controls
        controller->processInputs(deltaTime);
    }

    void RaytracingApplication::render(const float deltaTime)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        renderer.render();
    }

    void RaytracingApplication::destroy() noexcept
    {

    }
}

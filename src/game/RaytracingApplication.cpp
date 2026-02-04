#include "RaytracingApplication.h"

#include "scene/Components.h"

namespace application
{
    void RaytracingApplication::init()
    {
        renderer.init(window);
        controller = std::make_unique<CameraController>(renderer.getCamera(), window.getInputsManager());

        interface = std::make_shared<Interface>(&window, &renderer, this);
        window.registerImGuiWindow(interface);

        scene.newEntity("Empty Entity");
        scene.newEntity("Moveable Entity").addComponent<TransformComponent>();
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

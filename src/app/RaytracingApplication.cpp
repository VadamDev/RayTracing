#include "RaytracingApplication.h"

#include "../engine/scene/Entity.h"
#include "scene/Components.h"

namespace application
{
    void RaytracingApplication::init()
    {
        modelManager.loadAll();

        renderer.init(&window, this);
        controller = std::make_unique<CameraController>(renderer.getCamera(), window.getInputsManager());

        interface = std::make_shared<Interface>(&window, &renderer, this, controller.get());
        window.registerImGuiWindow(interface);

        sceneSerializer.addSerializableComponent<engine::TagComponent>();

        sceneSerializer.addSerializableComponent<TransformComponent>();
        sceneSerializer.addSerializableComponent<RaytracedMaterialComponent>();
        sceneSerializer.addSerializableComponent<RaytracedSphereComponent>();
        sceneSerializer.addSerializableComponent<RaytracedBoxComponent>();
        sceneSerializer.addSerializableComponent<RaytracedMeshComponent>();
    }

    void RaytracingApplication::update()
    {

    }

    void RaytracingApplication::processInputs(const float deltaTime)
    {
        if (!window.isGrabbed())
            return;

        const auto inputsManager = window.getInputsManager();

        // Ungrab Viewport
        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_ESCAPE))
            window.setGrabbed(false);

        // Camera controls
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

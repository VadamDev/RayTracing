#include "RaytracingApplication.h"

#include <spdlog/spdlog.h>

#include "../engine/scene/Entity.h"
#include "scene/Components.h"
#include "../engine/scene/Scene.h"

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

        try
        {
            scene = sceneSerializer.deserializeScene("resources/assets/scenes/default_scene.json", &globalMessenger);
        }
        catch (std::exception &e)
        {
            spdlog::warn("Failed to load default scene: {}", e.what());
            scene = new engine::Scene(&globalMessenger);
        }
    }

    void RaytracingApplication::update()
    {

    }

    void RaytracingApplication::processInputs(const float deltaTime)
    {
        if (!window.isGrabbed())
            return;

        const auto &inputsManager = window.getInputsManager();

        // Ungrab Viewport
        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_ESCAPE))
            window.setGrabbed(false);

        // Camera controls
        controller->processInputs(deltaTime);
    }

    void RaytracingApplication::render(const float deltaTime)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // We're delaying scene change to next frame because UI might still use the old scene pointer. TODO: use ref?
        if (nextScene != nullptr)
        {
            delete scene;
            scene = nextScene;
            nextScene = nullptr;

            UpdateShaderBuffersEvent event;
            globalMessenger.dispatch(event);
        }

        renderer.render();
    }

    void RaytracingApplication::destroy() noexcept
    {
        delete scene;
    }

    void RaytracingApplication::loadScene(engine::Scene *scene)
    {
        nextScene = scene;
    }
}

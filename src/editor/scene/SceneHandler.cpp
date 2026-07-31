#include "SceneHandler.h"

#include "../../engine/scene/Entity.h"
#include "Components.h"

namespace editor
{
    SceneHandler::SceneHandler(engine::Messenger &globalMessenger)
        : globalMessenger(globalMessenger)
    {
        // Register components that we want to save
        serializer.registerSerializable<engine::TagComponent>();
        serializer.registerSerializable<TransformComponent>();
        serializer.registerSerializable<CameraComponent>();
        serializer.registerSerializable<RaytracedMaterialComponent>();
        serializer.registerSerializable<RaytracedSphereComponent>();
        serializer.registerSerializable<RaytracedMeshComponent>();
    }

    void SceneHandler::openScene(std::unique_ptr<engine::Scene> scene)
    {
        sceneToOpen = std::move(scene);
    }

    void SceneHandler::openScene(const std::string &path)
    {
        openScene(serializer.deserializeScene(path, &globalMessenger));
    }

    void SceneHandler::openNewEmptyScene()
    {
        auto scene = std::make_unique<engine::Scene>(&globalMessenger);

        engine::Entity camera = scene->newEntity("Camera");
        camera.addComponent<TransformComponent>();
        camera.addComponent<CameraComponent>().primary = true;

        openScene(std::move(scene));
    }

    void SceneHandler::saveCurrentScene(const std::string &path)
    {
        if (!isSceneOpened())
            return;

        serializer.serializeScene(currentScene.get(), path);
    }

    void SceneHandler::closeCurrentScene()
    {
        if (currentScene == nullptr)
            return;

        shouldCloseCurrentScene = true;
    }

    void SceneHandler::onFramePush() noexcept
    {
        if (shouldCloseCurrentScene)
        {
            currentScene = nullptr; // shouldCloseCurrentScene should only be set to true IF currentScene != nullptr
            return;
        }

        if (sceneToOpen != nullptr)
            currentScene = std::move(sceneToOpen);
    }
}

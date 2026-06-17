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

    SceneHandler::~SceneHandler()
    {
        // Scene memory is currently managed manually TODO: use smart pointers
        if (currentScene != nullptr)
        {
            delete currentScene;
            currentScene = nullptr;
        }

        if (sceneToOpen != nullptr)
        {
            delete sceneToOpen;
            sceneToOpen = nullptr;
        }
    }

    void SceneHandler::openScene(engine::Scene *scene)
    {
        sceneToOpen = scene;
    }

    void SceneHandler::openScene(const std::string &path)
    {
        engine::Scene *loadedScene = serializer.deserializeScene(path, &globalMessenger);
        openScene(loadedScene);
    }

    void SceneHandler::openNewEmptyScene()
    {
        auto *scene = new engine::Scene(&globalMessenger);

        engine::Entity camera = scene->newEntity("Camera");
        camera.addComponent<TransformComponent>();
        camera.addComponent<CameraComponent>().primary = true;

        openScene(scene);
    }

    void SceneHandler::saveCurrentScene(const std::string &path)
    {
        if (!isSceneOpened())
            return;

        serializer.serializeScene(currentScene, path);
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
            delete currentScene; // shouldCloseCurrentScene should only be set to true IF currentScene != nullptr
            currentScene = nullptr;

            return;
        }

        if (sceneToOpen != nullptr)
        {
            if (currentScene != nullptr)
                delete currentScene;

            currentScene = sceneToOpen;
            sceneToOpen = nullptr;
        }
    }
}

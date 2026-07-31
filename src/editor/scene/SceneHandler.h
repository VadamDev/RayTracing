#pragma once

#include "../../engine/serial/SceneSerializer.h"

namespace engine
{
    class Messenger;
}

namespace editor
{
    class SceneHandler
    {

    public:
        explicit SceneHandler(engine::Messenger &globalMessenger);

        /*
         * Management
         */

        void openScene(std::unique_ptr<engine::Scene> scene);
        void openScene(const std::string &path);
        void openNewEmptyScene();

        void saveCurrentScene(const std::string &path);

        void closeCurrentScene();

        void onFramePush() noexcept;

        /*
         * Getters
         */

        bool isSceneOpened() const { return currentScene != nullptr; }
        engine::Scene* getOpenedScene() const { return isSceneOpened() ? currentScene.get() : nullptr; }

        engine::SceneSerializer& getSerializer() { return serializer; }
        engine::Messenger& getGlobalMessenger() const { return globalMessenger; }

    private:
        engine::Messenger &globalMessenger;
        engine::SceneSerializer serializer;

        // Since the current scene ptr will be modified DURING a frame, it's important to switch scene AFTER everything has been rendered
        std::unique_ptr<engine::Scene> currentScene = nullptr, sceneToOpen = nullptr;
        bool shouldCloseCurrentScene = false;
    };
}

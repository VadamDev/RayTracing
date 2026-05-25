#pragma once

#include "../../engine/scene/Scene.h"
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
        explicit SceneHandler(engine::Messenger *messenger);
        ~SceneHandler();

        /*
         * Management
         */

        void openScene(engine::Scene *scene);
        void openScene(const std::string &path);
        void openNewEmptyScene();

        void saveCurrentScene(const std::string &path);

        void closeCurrentScene();

        void onFramePush() noexcept;

        /*
         * Getters
         */

        bool isSceneOpened() const { return currentScene != nullptr; }
        engine::Scene* getOpenedScene() const { return currentScene; }

        engine::SceneSerializer& getSerializer() { return serializer; }

    private:
        engine::Messenger *messenger;
        engine::SceneSerializer serializer;

        // Since the current scene ptr will be modified DURING a frame, its important to switch scene AFTER everything has been rendered
        engine::Scene *currentScene = nullptr, *sceneToOpen = nullptr;
        bool shouldCloseCurrentScene = false;
    };
}

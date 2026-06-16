#pragma once

#include <glm/vec3.hpp>

#include "ICamController.h"

namespace engine
{
    class InputsManager;
}

namespace editor
{
    class FreecamController : public ICamController
    {

    public:
        explicit FreecamController(CameraSystem *cameraSystem, engine::InputsManager &inputsManager)
            : ICamController(cameraSystem), inputsManager(inputsManager) {}

        bool processInputs(float deltaTime) override;

        float sensitivity = 0.2f;
        float cameraSpeed = 5;
    private:
        engine::InputsManager &inputsManager;

        bool moved = false;

        void processMouse(glm::vec3 &cameraRot);
        void processKeyboard(glm::vec3 &cameraPos, const glm::vec3 &cameraRot, float deltaTime);
    };
}

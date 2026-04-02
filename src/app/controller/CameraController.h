#pragma once

#include <memory>
#include "../../engine/window/input/InputsManager.h"
#include "../rendering/Camera.h"

namespace application
{
    class CameraController
    {
    public:
        explicit CameraController(Camera *camera, const std::shared_ptr<engine::InputsManager> &inputsManager)
            : camera(camera), inputsManager(inputsManager) {}

        void processInputs(float deltaTime) const;

        float sensitivity = 0.2f;
        float cameraSpeed = 5;

    private:
        Camera *camera;
        std::shared_ptr<engine::InputsManager> inputsManager;

        bool processMouseMovements() const;
        bool processKeyboardMovements(float deltaTime) const;

        void move(glm::vec3 offset) const;
    };
}

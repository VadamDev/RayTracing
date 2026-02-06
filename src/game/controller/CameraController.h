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

        void processInputs(float deltaTime);

        float sensitivity = 0.2f;
        float cameraSpeed = 6;

    private:
        Camera *camera;
        std::shared_ptr<engine::InputsManager> inputsManager;

        bool processMouseMovements() const;
        bool processKeyboardMovements(float deltaTime) const;

        void move(float xOffset, float yOffset, float zOffset) const;
    };
}

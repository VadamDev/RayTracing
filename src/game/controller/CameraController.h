#pragma once

#include "../rendering/camera/Camera.h"

namespace game
{
    class CameraController
    {
    public:
        explicit CameraController(Camera *camera, const std::shared_ptr<engine::InputsManager> &inputsManager)
            : camera(camera), inputsManager(inputsManager) {}

        void processInputs(float deltaTime);

    private:
        Camera *camera;
        std::shared_ptr<engine::InputsManager> inputsManager;

        bool processMouseMovements() const;
        bool processKeyboardMovements(float deltaTime) const;
    };
}

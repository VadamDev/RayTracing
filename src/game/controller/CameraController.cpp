#include "CameraController.h"

namespace game
{
    static constexpr float SENSITIVITY = 0.2f;

    void CameraController::processInputs(float deltaTime)
    {
        const bool processedMouse = processMouseMovements(), processedKeyboard = processKeyboardMovements(deltaTime);

        if (processedMouse || processedKeyboard)
            camera->updateLocalToWorldMatrix();
    }

    bool CameraController::processMouseMovements() const
    {
        bool hasProcessed = false;

        const glm::dvec2 &mouseDelta = inputsManager->getMouseDelta();
        glm::vec2 &rotation = camera->rotation;

        if (mouseDelta.x != 0)
        {
            rotation.y += mouseDelta.x * SENSITIVITY;
            hasProcessed = true;
        }

        if (mouseDelta.y != 0)
        {
            rotation.x += mouseDelta.y * SENSITIVITY;
            hasProcessed = true;
        }

        if (hasProcessed)
        {
            if (rotation.x > 90)
                rotation.x = 90;
            else if (rotation.x < -90)
                rotation.x = -90;

            if (rotation.y > 360)
                rotation.y = 0;
            else if (rotation.y < 0)
                rotation.y += 360;
        }

        return hasProcessed;
    }

    bool CameraController::processKeyboardMovements(float deltaTime) const
    {
        bool hasProcessed = false;

        //TODO: keyboard movements

        return hasProcessed;
    }
}

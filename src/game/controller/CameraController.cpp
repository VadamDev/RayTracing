#include "CameraController.h"

namespace application
{
    static constexpr glm::vec3 WORLD_UP(0, 1, 0);

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
            rotation.y += mouseDelta.x * sensitivity;
            hasProcessed = true;
        }

        if (mouseDelta.y != 0)
        {
            rotation.x += mouseDelta.y * sensitivity;
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
        float xOffset = 0, yOffset = 0, zOffset = 0;

        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_W))
            zOffset += cameraSpeed;

        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_A))
            xOffset -= cameraSpeed;

        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_S))
            zOffset -= cameraSpeed;

        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_D))
            xOffset += cameraSpeed;

        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_SPACE))
            yOffset += cameraSpeed;

        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_LEFT_CONTROL))
            yOffset -= cameraSpeed;

        if (xOffset != 0 || yOffset != 0 || zOffset != 0)
        {
            move(xOffset * deltaTime, yOffset * deltaTime, zOffset * deltaTime);
            return true;
        }

        return false;
    }

    void CameraController::move(const float xOffset, const float yOffset, const float zOffset) const
    {
        const float yawRad = glm::radians(camera->rotation.y);

        const glm::vec3 forward(glm::sin(yawRad), 0, glm::cos(yawRad));
        const glm::vec3 right = glm::cross(WORLD_UP, forward);

        glm::vec3 &pos = camera->position;
        pos += forward * zOffset;
        pos += right * xOffset;
        pos += WORLD_UP * yOffset;
    }
}

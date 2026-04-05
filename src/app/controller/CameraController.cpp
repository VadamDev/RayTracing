#include "CameraController.h"

namespace application
{
    static constexpr glm::vec3 ZERO(0, 0, 0);
    static constexpr glm::vec3 WORLD_UP(0, 1, 0);

    static constexpr float SPRINT_MULTIPLIER = 2.5f;

    void CameraController::processInputs(const float deltaTime) const
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
                rotation.y -= 360;
            else if (rotation.y < 0)
                rotation.y += 360;
        }

        return hasProcessed;
    }

    bool CameraController::processKeyboardMovements(const float deltaTime) const
    {
        glm::vec3 offset(0, 0, 0);
        bool sprinting = false;

        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_W))
            offset.z += 1;

        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_A))
            offset.x -= 1;

        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_S))
            offset.z -= 1;

        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_D))
            offset.x += 1;

        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_SPACE))
            offset.y += 1;

        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_LEFT_CONTROL))
            offset.y -= 1;

        if (inputsManager->isKeyDown(engine::KeyboardKeys::KEY_LEFT_SHIFT))
            sprinting = true;

        if (offset != ZERO)
        {
            offset = glm::normalize(offset) * cameraSpeed * (sprinting ? SPRINT_MULTIPLIER : 1) * deltaTime;
            move(offset);

            return true;
        }

        return false;
    }

    void CameraController::move(const glm::vec3 offset) const
    {
        const float yawRad = glm::radians(camera->rotation.y);

        const glm::vec3 forward(glm::sin(yawRad), 0, glm::cos(yawRad));
        const glm::vec3 right = glm::cross(WORLD_UP, forward);

        glm::vec3 &pos = camera->position;
        pos += offset.x * right;
        pos += offset.y * WORLD_UP;
        pos += offset.z * forward;
    }
}

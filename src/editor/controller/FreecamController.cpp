#include "FreecamController.h"

#include "../rendering/CameraSystem.h"
#include "../../engine/window/inputs/InputsManager.h"

namespace editor
{
    static constexpr glm::vec3 ZERO(0, 0, 0);
    static constexpr glm::vec3 WORLD_UP(0, 1, 0);

    static constexpr float SPRINT_MULTIPLIER = 2.5f;

    void FreecamController::processInputs(const float deltaTime)
    {
        TransformComponent *transform = cameraSystem->getPrimaryCamera().transform;
        processMouse(transform->rotation);
        processKeyboard(transform->position, transform->rotation, deltaTime);

        if (moved)
        {
            cameraSystem->updateLocalToWorldMatrix();
            moved = false;
        }
    }

    void FreecamController::processMouse(glm::vec3 &cameraRot)
    {
        bool hasProcessed = false;

        const glm::dvec2 &mouseDelta = inputsManager.getMouseDelta();

        if (mouseDelta.x != 0)
        {
            cameraRot.y += mouseDelta.x * sensitivity;
            hasProcessed = true;
        }

        if (mouseDelta.y != 0)
        {
            cameraRot.x += mouseDelta.y * sensitivity;
            hasProcessed = true;
        }

        if (hasProcessed)
        {
            if (cameraRot.x > 90)
                cameraRot.x = 90;
            else if (cameraRot.x < -90)
                cameraRot.x = -90;

            if (cameraRot.y > 360)
                cameraRot.y -= 360;
            else if (cameraRot.y < 0)
                cameraRot.y += 360;
        }

        moved = hasProcessed;
    }

    void FreecamController::processKeyboard(glm::vec3 &cameraPos, const glm::vec3 &cameraRot, const float deltaTime)
    {
        glm::vec3 offset(0, 0, 0);

        if (inputsManager.isKeyDown(engine::KeyboardKeys::KEY_W))
            offset.z += 1;

        if (inputsManager.isKeyDown(engine::KeyboardKeys::KEY_A))
            offset.x -= 1;

        if (inputsManager.isKeyDown(engine::KeyboardKeys::KEY_S))
            offset.z -= 1;

        if (inputsManager.isKeyDown(engine::KeyboardKeys::KEY_D))
            offset.x += 1;

        if (inputsManager.isKeyDown(engine::KeyboardKeys::KEY_SPACE))
            offset.y += 1;

        if (inputsManager.isKeyDown(engine::KeyboardKeys::KEY_LEFT_CONTROL))
            offset.y -= 1;

        bool sprinting = false;
        if (inputsManager.isKeyDown(engine::KeyboardKeys::KEY_LEFT_SHIFT))
            sprinting = true;

        if (offset == ZERO)
            return;

        offset = glm::normalize(offset) * cameraSpeed * (sprinting ? SPRINT_MULTIPLIER : 1) * deltaTime;

        const float yawRad = glm::radians(cameraRot.y);

        const glm::vec3 forward(glm::sin(yawRad), 0, glm::cos(yawRad));
        const glm::vec3 right = glm::cross(WORLD_UP, forward);

        cameraPos += offset.x * right;
        cameraPos += offset.y * WORLD_UP;
        cameraPos += offset.z * forward;

        moved = true;
    }
}

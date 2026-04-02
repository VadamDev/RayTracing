#include "Camera.h"

#include "Renderer.h"
#include "../../engine/messenger/Messenger.hpp"

namespace application
{
    static constexpr glm::vec3 RIGHT(1, 0, 0);
    static constexpr glm::vec3 UP   (0, 1, 0);

    Camera::Camera(const float fov, const float nearClipPlane, engine::Messenger *messenger)
        : fov(fov), nearClipPlane(nearClipPlane), messenger(messenger)
    {
        messenger->subscribe<engine::WindowResizeEvent>([this](const engine::WindowResizeEvent *event) {
            this->windowWidth = event->newWidth;
            this->windowHeight = event->newHeight;

            this->aspectRatio = event->newAspectRatio;
        });
    }

    void Camera::updateLocalToWorldMatrix()
    {
        localToWorldMatrix = glm::identity<glm::mat4>();
        localToWorldMatrix = glm::translate(localToWorldMatrix, position);
        localToWorldMatrix = glm::rotate(localToWorldMatrix, glm::radians(rotation.y), UP);
        localToWorldMatrix = glm::rotate(localToWorldMatrix, glm::radians(rotation.x), RIGHT);

        CameraMovedEvent event;
        messenger->dispatch(event);
    }
}

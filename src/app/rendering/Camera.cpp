#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_projection.hpp>

#include "Renderer.h"
#include "../../engine/messenger/Messenger.hpp"

namespace application
{
    static constexpr glm::vec3 RIGHT(1, 0, 0);
    static constexpr glm::vec3 UP   (0, 1, 0);

    Camera::Camera(const float fov, const float nearClipPlane, engine::Messenger *messenger)
        : fov(fov), nearClipPlane(nearClipPlane), messenger(messenger)
    {
        messenger->subscribe<CanvasResizeEvent>([this](const CanvasResizeEvent *event) {
            this->windowWidth = event->newWidth;
            this->windowHeight = event->newHeight;

            this->aspectRatio = event->newAspectRatio;

            projMatrix = glm::perspective(glm::atan(glm::tan(glm::radians(this->fov / 2.0f)) / 2) * 2, aspectRatio, this->nearClipPlane, 1000.f); //TODO: change me!
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

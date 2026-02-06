#include "Camera.h"

#include "Renderer.h"

namespace application
{
    static constexpr glm::vec3 UP(0, 1, 0);
    static constexpr glm::vec3 RIGHT(1, 0, 0);

    Camera::Camera(const float fov, const float nearClipPlane, Renderer *renderer)
        : fov(fov), nearClipPlane(nearClipPlane)
    {
        renderer->onViewportResize([this](const engine::WindowResizeEvent &event) {
            this->windowWidth = event.getNewWidth();
            this->windowHeight = event.getNewHeight();

            this->aspectRatio = event.getNewAspectRatio();
        });
    }

    void Camera::updateLocalToWorldMatrix()
    {
        localToWorldMatrix = glm::identity<glm::mat4>();
        localToWorldMatrix = glm::translate(localToWorldMatrix, position);
        localToWorldMatrix = glm::rotate(localToWorldMatrix, glm::radians(rotation.y), UP);
        localToWorldMatrix = glm::rotate(localToWorldMatrix, glm::radians(rotation.x), RIGHT);
    }
}

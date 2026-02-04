#include "Camera.h"

namespace application
{
    static constexpr glm::vec3 UP(0, 1, 0);
    static constexpr glm::vec3 RIGHT(1, 0, 0);

    void Camera::updateLocalToWorldMatrix()
    {
        localToWorldMatrix = glm::identity<glm::mat4>();
        localToWorldMatrix = glm::translate(localToWorldMatrix, position);
        localToWorldMatrix = glm::rotate(localToWorldMatrix, glm::radians(rotation.y), UP);
        localToWorldMatrix = glm::rotate(localToWorldMatrix, glm::radians(rotation.x), RIGHT);
    }

    void Camera::registerCallbacks(engine::Window *window)
    {
        window->whenResized([this](const engine::WindowResizeEvent &event) {
            this->windowWidth = event.getNewWidth();
            this->windowHeight = event.getNewHeight();

            this->aspectRatio = event.getNewAspectRatio();
        });
    }
}

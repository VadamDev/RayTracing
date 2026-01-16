#include "Camera.h"

namespace game
{
    void Camera::updateLocalToWorldMatrix()
    {
        localToWorldMatrix = glm::identity<glm::mat4>();
        localToWorldMatrix = glm::translate(localToWorldMatrix, position);
        localToWorldMatrix = glm::rotate(localToWorldMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        localToWorldMatrix = glm::rotate(localToWorldMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    }

    void Camera::registerCallbacks(engine::Window *window)
    {
        window->whenResized([this](const engine::WindowResizeEvent &event) {
            this->aspectRatio = event.getNewAspectRatio();
        });
    }
}

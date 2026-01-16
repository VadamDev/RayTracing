#pragma once

#include "../../../engine/window/Window.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace game
{
    class Camera
    {

    public:
        explicit Camera(const float fov, const float nearClipPlane, engine::Window *window)
            : fov(fov), nearClipPlane(nearClipPlane)
        {
            registerCallbacks(window);
        }

        void updateLocalToWorldMatrix();

        float getAspectRatio() const { return aspectRatio; }
        glm::mat4& getLocalToWorldMatrix() { return localToWorldMatrix; }

        float fov, nearClipPlane;

        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::vec2 rotation = glm::vec2(0, 0);

    private:
        float aspectRatio;

        glm::mat4 localToWorldMatrix = glm::identity<glm::mat4>();;

        void registerCallbacks(engine::Window *window);
    };
}

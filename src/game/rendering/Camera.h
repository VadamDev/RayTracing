#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace application
{
    static constexpr float DEFAULT_ASPECT_RATIO = 16.0f / 9.0f;

    class Renderer;

    class Camera
    {

    public:
        explicit Camera(float fov, float nearClipPlane, Renderer *renderer);

        void updateLocalToWorldMatrix();

        int getWindowWidth() const { return windowWidth; }
        int getWindowHeight() const { return windowHeight; }
        float getAspectRatio() const { return aspectRatio; }

        glm::mat4& getLocalToWorldMatrix() { return localToWorldMatrix; }

        float fov, nearClipPlane;
        float targetAspectRatio = DEFAULT_ASPECT_RATIO;

        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::vec2 rotation = glm::vec2(0, 0);

    private:
        int windowWidth = 0, windowHeight = 0;
        float aspectRatio = DEFAULT_ASPECT_RATIO;

        glm::mat4 localToWorldMatrix = glm::identity<glm::mat4>();
    };
}

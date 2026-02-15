#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace application
{
    struct TransformComponent
    {
        glm::vec3 position;
        glm::vec3 rotation;
        float scale;
    };

    struct RaytracedMaterialComponent
    {
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    };

    struct alignas(16) RaytracedSphereComponent
    {
        glm::vec3 position = glm::vec3(0, 0, 0);
        float radius = 0;
        RaytracedMaterialComponent material = {};
    };
}

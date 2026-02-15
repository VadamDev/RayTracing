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
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
        float pad0 = 0; //GLSL enforces 16bit padding
        glm::vec3 emissionColor = glm::vec3(0, 0, 0);
        float emissionStrength = 0;
    };

    struct alignas(16) RaytracedSphereComponent
    {
        glm::vec3 position = glm::vec3(0, 0, 0);
        float radius = 0;
        RaytracedMaterialComponent material = {};
    };
}

#pragma once

#include <glm/vec3.hpp>

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
        float pad0 = 0;
        glm::vec3 emissionColor = glm::vec3(0, 0, 0);
        float emissionStrength = 0;
        float smoothness = 0;
    };

    struct alignas(16) RaytracedSphereComponent
    {
        glm::vec3 position = glm::vec3(0, 0, 0);
        float radius = 0;
        RaytracedMaterialComponent material = {};
    };

    struct alignas(16) RaytracedBoxComponent
    {
        glm::vec3 boxMin = glm::vec3(0, 0, 0);
        float pad0 = 0;
        glm::vec3 boxMax = glm::vec3(0, 0, 0);
        float pad1 = 0;
        RaytracedMaterialComponent material = {};
    };
}

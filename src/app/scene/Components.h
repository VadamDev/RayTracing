#pragma once

#include <glm/vec3.hpp>
#include <string>

namespace application
{
    struct TransformComponent
    {
        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::vec3 rotation = glm::vec3(0, 0, 0);
        glm::vec3 scale = glm::vec3(1);
    };

    struct alignas(16) RaytracedMaterialComponent
    {
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
        float smoothness = 0;

        glm::vec3 emissionColor = glm::vec3(0, 0, 0);
        float emissionStrength = 0;

        int type = 0;
    };

    struct alignas(16) RaytracedSphereComponent
    {
        glm::vec3 position = glm::vec3(0, 0, 0);
        float radius = 0;
        RaytracedMaterialComponent material{};
    };

    struct alignas(16) RaytracedBoxComponent
    {
        glm::vec3 boxMin = glm::vec3(0, 0, 0); float pad0 = 0;
        glm::vec3 boxMax = glm::vec3(0, 0, 0); float pad1 = 0;
        RaytracedMaterialComponent material{};
    };

    struct RaytracedMeshComponent
    {
        std::string name;
    };

    struct alignas(16) TriangleMeshData
    {
        unsigned int triIndex;
        unsigned int triCount;

        float pad0 = 0, pad1 = 0;

        glm::vec3 boxMin; float pad2 = 0;
        glm::vec3 boxMax; float pad3 = 0;

        glm::mat4 localToWorld;
        glm::mat4 worldToLocal;

        RaytracedMaterialComponent material{};
    };
}

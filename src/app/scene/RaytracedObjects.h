#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace application {
    // Material
    struct alignas(16) RaytracedMaterial
    {
        glm::vec3 color = glm::vec3(1);
        float smoothness = 0;

        glm::vec3 specularColor = glm::vec3(1);
        float specularProbability = 0;

        glm::vec3 emissionColor = glm::vec3(0);
        float emissionStrength = 0;

        int type = 0;
    };

    // Sphere
    struct alignas(16) RaytracedSphere
    {
        glm::vec3 position = glm::vec3(0);
        float radius = 0;

        RaytracedMaterial material {};
    };

    // Box
    struct alignas(16) RaytracedBox
    {
        glm::vec3 boxMin = glm::vec3(0); float pad0 = 0;
        glm::vec3 boxMax = glm::vec3(0); float pad1 = 0;

        RaytracedMaterial material {};
    };

    // Triangles
    struct alignas(16) RaytracedBVHNode {
        glm::vec3 boxMin = glm::vec3(0); float pad0 = 0;
        glm::vec3 boxMax = glm::vec3(0);

        int leftChildIdx = 0;
        int triIndex = 0, triCount = 0;
    };
    
    struct alignas(16) RaytracedTriangle
    {
        glm::vec3 posA; float pad0 = 0;
        glm::vec3 posB; float pad1 = 0;
        glm::vec3 posC; float pad2 = 0;

        glm::vec3 normalA; float pad3 = 0;
        glm::vec3 normalB; float pad4 = 0;
        glm::vec3 normalC;
    };

    struct alignas(16) TriangleMeshData
    {
        int rootBVHNodeIndex = 0; float pad0[3];

        glm::mat4 localToWorld;
        glm::mat4 worldToLocal;

        RaytracedMaterial material {};
    };
}

#pragma once

#include <glm/vec3.hpp>
#include <algorithm>
#include "../scene/RaytracedObjects.h"

namespace application {
    namespace bvh_utils
    {
        inline glm::vec3 min(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c)
        {
            return {
                std::min(a.x, std::min(b.x, c.x)),
                std::min(a.y, std::min(b.y, c.y)),
                std::min(a.z, std::min(b.z, c.z)),
            };
        }

        inline glm::vec3 max(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c)
        {
            return {
                std::max(a.x, std::max(b.x, c.x)),
                std::max(a.y, std::max(b.y, c.y)),
                std::max(a.z, std::max(b.z, c.z)),
            };
        }
    }

    struct BVHTriangle
    {
        glm::vec3 posA, posB, posC;
        glm::vec3 normalA, normalB, normalC;

        glm::vec3 centroid;
        glm::vec3 minPos, maxPos;

        explicit BVHTriangle(const glm::vec3 &posA, const glm::vec3 &posB, const glm::vec3 &posC, const glm::vec3 normalA, const glm::vec3 normalB, const glm::vec3 normalC)
            : posA(posA), posB(posB), posC(posC),
              normalA(normalA), normalB(normalB), normalC(normalC),
              centroid((posA + posB + posC) / 3.0f),
              minPos(bvh_utils::min(posA, posB, posC)), maxPos(bvh_utils::max(posA, posB, posC)) {}

        explicit operator RaytracedTriangle() const
        {
            return {
                .posA = posA,
                .posB = posB,
                .posC = posC,

                .normalA = normalA,
                .normalB = normalB,
                .normalC = normalC
            };
        }
    };
}

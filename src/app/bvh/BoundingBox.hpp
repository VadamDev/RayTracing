#pragma once

#include <glm/vec3.hpp>

namespace application
{
    class BoundingBox
    {

    public:
        glm::vec3 boxMin, boxMax;
        glm::vec3 size = glm::vec3(0), center = glm::vec3(0);

        explicit BoundingBox()
            : boxMin(std::numeric_limits<float>::infinity()), boxMax(-std::numeric_limits<float>::infinity())
        {
            update();
        }

        void growToInclude(const glm::vec3 &minPos, const glm::vec3 &maxPos)
        {
            boxMin.x = std::min(boxMin.x, minPos.x);
            boxMin.y = std::min(boxMin.y, minPos.y);
            boxMin.z = std::min(boxMin.z, minPos.z);

            boxMax.x = std::max(boxMax.x, maxPos.x);
            boxMax.y = std::max(boxMax.y, maxPos.y);
            boxMax.z = std::max(boxMax.z, maxPos.z);

            update();
        }

        float getArea() const
        {
            return size.x * size.y + size.y * size.z + size.z * size.x;
        }

    private:
        void update()
        {
            size = boxMax - boxMin;
            center = boxMin + size * 0.5f;
        }
    };
}

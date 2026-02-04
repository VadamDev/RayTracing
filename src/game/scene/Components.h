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
}

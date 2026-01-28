#pragma once

#include "entt/entt.hpp"

namespace engine
{
    class Entity;

    class Scene
    {

    public:
        Entity newEntity();

        entt::registry registry;
    };
}

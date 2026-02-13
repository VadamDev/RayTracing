#pragma once

#include "entt/entt.hpp"

namespace engine
{
    class Entity;

    class Scene
    {

    public:
        Entity newEntity(const std::string &name);
        Entity copyEntity(const Entity &source, const std::string &name);
        void destroyEntity(const Entity &entity);

        entt::registry registry;
    };
}

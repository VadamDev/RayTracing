#pragma once

#include "entt/entt.hpp"

namespace engine
{
    class Entity;
    class Messenger;

    struct AddEntityToSceneEvent
    {
        explicit AddEntityToSceneEvent(const Entity *entity)
            : entity(entity) {}

        const Entity *entity;
    };

    struct RemoveEntityFromSceneEvent
    {
        explicit RemoveEntityFromSceneEvent(const Entity *entity)
            : entity(entity) {}

        const Entity *entity;
    };

    class Scene
    {

    public:
        explicit Scene(Messenger *globalMessenger)
            : globalMessenger(globalMessenger) {}

        Entity newEntity(const std::string &name);
        Entity copyEntity(const Entity &source, const std::string &name);
        void destroyEntity(const Entity &entity);

        entt::registry registry;

    private:
        Messenger *globalMessenger;
    };
}

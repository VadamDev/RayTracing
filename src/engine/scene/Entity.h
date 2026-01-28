#pragma once

#include "Scene.h"

namespace engine
{
    class Entity
    {

    public:
        Entity(const entt::entity handle, Scene *scene)
            : handle(handle), scene(scene) {}

        template<typename T, typename... Args>
        T& addComponent(Args&&... args)
        {
            return scene->registry.emplace<T>(handle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& getComponent()
        {
            return scene->registry.get<T>(handle);
        }

        template<typename T>
        void removeComponent() const
        {
            scene->registry.remove<T>(handle);
        }

        template<typename... T>
        bool hasComponents() const
        {
            return scene->registry.all_of<T>(handle);
        }

    private:
        entt::entity handle;
        Scene *scene; //TODO: it would probably be nicer to use weak_ptr here
    };
}

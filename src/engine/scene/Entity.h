#pragma once

#include <utility>

#include "Scene.h"

namespace engine
{
    struct TagComponent
    {
        std::string tag;

        explicit TagComponent(std::string tag)
            : tag(std::move(tag)) {}
    };

    class Entity
    {

    public:
        Entity() = default;
        Entity(const entt::entity handle, Scene *scene)
            : handle(handle), scene(scene) {}

        template<typename T, typename... Args>
        T& addComponent(Args&&... args) const
        {
            return scene->registry.emplace<T>(handle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& getComponent() const
        {
            return scene->registry.get<T>(handle);
        }

        template<typename T>
        void removeComponent() const
        {
            scene->registry.remove<T>(handle);
        }

        template<typename T>
        bool hasComponent() const
        {
            return scene->registry.all_of<T>(handle);
        }

        operator bool() const { return handle != entt::null && scene != nullptr; }
        operator uint32_t() const { return static_cast<uint32_t>(handle); }
        operator entt::entity() const { return handle; }

        bool operator==(const Entity &other) const { return handle == other.handle && scene == other.scene; }
        bool operator!=(const Entity &other) const { return !(*this == other); }

    private:
        entt::entity handle = entt::null;
        Scene *scene = nullptr; //TODO: it would probably be nicer to use weak_ptr here
    };
}

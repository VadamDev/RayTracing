#pragma once

#include <nlohmann/json.hpp>

#include "../scene/Scene.h"
#include "IJsonSerializable.h"

namespace engine {
    class Messenger;

    struct ComponentSerializerProxy
    {
        std::function<void(nlohmann::json&, const void*)> serializeFunc;
        std::function<void(entt::registry&, const entt::entity&, const nlohmann::json&)> deserializeFunc;
    };

    class SceneSerializer {

    public:
        void serializeScene(const Scene *scene, const std::string &name);
        Scene* deserializeScene(const std::string &path, Messenger *messenger);

        template<std::derived_from<IJsonSerializable> T>
        void registerSerializable()
        {
            const ComponentSerializerProxy proxy {
                .serializeFunc = [](nlohmann::json &componentJson, const void *rawComponent) {
                    const T *component = static_cast<const T*>(rawComponent);
                    component->serialize(componentJson);
                },

                .deserializeFunc = [](entt::registry &registry, const entt::entity &entity, const nlohmann::json &componentJson) {
                    T &component = registry.emplace<T>(entity);
                    component.deserialize(componentJson);
                }
            };

            componentSerializers.emplace(entt::type_id<T>().hash(), proxy);
        }

    private:
        std::unordered_map<entt::id_type, ComponentSerializerProxy> componentSerializers;
    };
}
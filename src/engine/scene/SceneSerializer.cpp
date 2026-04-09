#include "SceneSerializer.h"

#include <spdlog/spdlog.h>
#include <fstream>
#include "../messenger/Messenger.hpp"

namespace engine {
    using nlohmann::json;

    void SceneSerializer::serializeScene(const Scene *scene, const std::string &name)
    {
        json sceneJson = json::array();
        for (const auto &entityHandle : scene->registry.view<entt::entity>())
        {
            json entityJson;

            json componentsList = json::array();
            for (const auto&& [id, storage] : scene->registry.storage())
            {
                if (!storage.contains(entityHandle))
                    continue;

                const auto &pair = componentSerializers.find(id);
                if (pair == componentSerializers.end())
                {
                    spdlog::warn("Component with type id {} does not have a registered serializer! Skipping...", id);
                    continue;
                }

                const ComponentSerializerProxy &proxy = pair->second;

                json componentJson;
                componentJson["hs"] = id;
                proxy.serializeFunc(componentJson, storage.value(entityHandle));

                componentsList.push_back(componentJson);
            }

            entityJson["components"] = std::move(componentsList);
            sceneJson.push_back(entityJson);
        }

        std::ofstream file(name);
        file << sceneJson.dump(2) << std::endl;
        file.close();
    }

    Scene* SceneSerializer::deserializeScene(const std::string &path, Messenger *messenger)
    {

        std::ifstream file(path);
        if (!file.is_open())
            throw std::runtime_error("Failed to open scene file at: " + path);

        const json sceneJson = json::parse(file);
        file.close();

        auto *scene = new Scene(messenger);

        for(const auto& [_, entityJson] : sceneJson.items())
        {
            const entt::entity entityHandle = scene->registry.create();

            const json &componentsJson = entityJson.value("components", json::array());
            for (size_t i = 0; i < componentsJson.size(); i++)
            {
                const json &componentJson = componentsJson[i];
                const entt::id_type &componentTypeId = componentJson.value("hs", -1);

                if (componentTypeId == -1)
                {
                    spdlog::warn("Json does not contain a type id! Skipping...");
                    continue;
                }

                const auto &pair = componentSerializers.find(componentTypeId);
                if (pair == componentSerializers.end())
                {
                    spdlog::warn("Component with type id {} does not have a registered deserializer! Skipping...", componentTypeId);
                    continue;
                }

                const ComponentSerializerProxy &proxy = pair->second;
                proxy.deserializeFunc(scene->registry, entityHandle, componentJson);
            }
        }

        return scene;
    }
}

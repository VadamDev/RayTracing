#pragma once

#include <nlohmann/json.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace engine
{
    inline glm::vec2 valueVec2(const nlohmann::json &json, const std::string &key, const glm::vec2 &defaultValue)
    {
        if (!json.contains(key) || !json[key].is_array() || json[key].size() != 2)
            return defaultValue;

        return { json[key][0], json[key][1] };
    }

    inline glm::vec3 valueVec3(const nlohmann::json &json, const std::string &key, const glm::vec3 &defaultValue)
    {
        if (!json.contains(key) || !json[key].is_array() || json[key].size() != 3)
            return defaultValue;

        return { json[key][0], json[key][1], json[key][2] };
    }

    struct SerializableComponent
    {
        virtual ~SerializableComponent() = default;

        virtual void serialize(nlohmann::json &componentJson) const = 0;
        virtual void deserialize(const nlohmann::json &componentJson) = 0;
    };
}

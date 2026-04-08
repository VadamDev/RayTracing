#pragma once

#include <nlohmann/json.hpp>

namespace engine
{
    struct SerializableComponent
    {
        virtual ~SerializableComponent() = default;

        virtual void serialize(nlohmann::json &componentJson) const = 0;
        virtual void deserialize(const nlohmann::json &componentJson) = 0;
    };
}

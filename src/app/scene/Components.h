#pragma once

#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "../../engine/scene/SerializableComponent.h"
#include "RaytracedObjects.h"

namespace application
{
    struct TransformComponent : engine::SerializableComponent
    {
        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::vec3 rotation = glm::vec3(0, 0, 0);
        glm::vec3 scale = glm::vec3(1);

        glm::mat4 toTransformMat() const
        {
            const glm::mat4 translate = glm::translate(glm::identity<glm::mat4>(), position);
            const glm::mat4 rotate = glm::toMat4(glm::quat(glm::radians(rotation)));
            const glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), this->scale);

            return translate * rotate * scale;
        }

        void serialize(nlohmann::json &componentJson) const override
        {
            componentJson["position"] = { position.x, position.y, position.z };
            componentJson["rotation"] = { rotation.x, rotation.y, rotation.z };
            componentJson["scale"] = { scale.x, scale.y, scale.z };
        }

        void deserialize(const nlohmann::json &componentJson) override
        {
            position = engine::valueVec3(componentJson, "position", position);
            rotation = engine::valueVec3(componentJson, "rotation", rotation);
            scale = engine::valueVec3(componentJson, "scale", scale);
        }
    };

    struct RaytracedMaterialComponent : engine::SerializableComponent
    {
        RaytracedMaterial material;

        void serialize(nlohmann::json &componentJson) const override
        {
            componentJson["color"] = { material.color.x, material.color.y, material.color.z };
            componentJson["smoothness"] = material.smoothness;

            componentJson["specularColor"] = { material.specularColor.x, material.specularColor.y, material.specularColor.z };
            componentJson["specularProbability"] = material.specularProbability;

            componentJson["emissionColor"] = { material.emissionColor.x, material.emissionColor.y, material.emissionColor.z };
            componentJson["emissionStrength"] = material.emissionStrength;

            componentJson["type"] = material.type;
        }

        void deserialize(const nlohmann::json &componentJson) override
        {
            material.color = engine::valueVec3(componentJson, "color", material.color);
            material.smoothness = componentJson.value("smoothness", material.smoothness);

            material.specularColor = engine::valueVec3(componentJson, "specularColor", material.specularColor);
            material.specularProbability = componentJson.value("specularProbability", material.specularProbability);

            material.emissionColor = engine::valueVec3(componentJson, "emissionColor", material.emissionColor);
            material.emissionStrength = componentJson.value("emissionStrength", material.emissionStrength);

            material.type = componentJson.value("type", material.type);
        }
    };

    struct alignas(16) RaytracedSphereComponent : engine::SerializableComponent
    {
        RaytracedSphere sphere;

        void serialize(nlohmann::json &componentJson) const override {}
        void deserialize(const nlohmann::json &componentJson) override {}
    };

    struct RaytracedMeshComponent : engine::SerializableComponent
    {
        std::string name;

        void serialize(nlohmann::json &componentJson) const override
        {
            componentJson["name"] = name;
        }

        void deserialize(const nlohmann::json &componentJson) override
        {
            name = componentJson.value("name", name);
        }
    };
}

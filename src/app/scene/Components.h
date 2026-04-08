#pragma once

#include <string>

#include "../../engine/scene/SerializableComponent.h"
#include "RaytracedObjects.h"

namespace application
{
    struct TransformComponent : engine::SerializableComponent
    {
        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::vec3 rotation = glm::vec3(0, 0, 0);
        glm::vec3 scale = glm::vec3(1);

        void serialize(nlohmann::json &componentJson) const override
        {
            componentJson["position"] = { position.x, position.y, position.z };
            componentJson["rotation"] = { rotation.x, rotation.y, rotation.z };
            componentJson["scale"] = { scale.x, scale.y, scale.z };
        }

        void deserialize(const nlohmann::json &componentJson) override
        {
            position = { componentJson["position"][0], componentJson["position"][1], componentJson["position"][2] };
            rotation = { componentJson["rotation"][0], componentJson["rotation"][1], componentJson["rotation"][2] };
            scale = { componentJson["scale"][0], componentJson["scale"][1], componentJson["scale"][2] };
        }
    };



    struct RaytracedMaterialComponent : engine::SerializableComponent
    {
        RaytracedMaterial material;

        void serialize(nlohmann::json &componentJson) const override
        {
            componentJson["color"] = { material.color.x, material.color.y, material.color.z };
            componentJson["smoothness"] = material.smoothness;

            componentJson["emissionColor"] = { material.emissionColor.x, material.emissionColor.y, material.emissionColor.z };
            componentJson["emissionStrength"] = material.emissionStrength;

            componentJson["type"] = material.type;
        }

        void deserialize(const nlohmann::json &componentJson) override
        {
            material.color = { componentJson["color"][0], componentJson["color"][1], componentJson["color"][2] };
            material.smoothness = componentJson["smoothness"];

            material.emissionColor = { componentJson["emissionColor"][0], componentJson["emissionColor"][1], componentJson["emissionColor"][2] };
            material.emissionStrength = componentJson["emissionStrength"];

            material.type = componentJson["type"];
        }
    };

    struct alignas(16) RaytracedSphereComponent : engine::SerializableComponent
    {
        RaytracedSphere sphere;

        void serialize(nlohmann::json &componentJson) const override {}
        void deserialize(const nlohmann::json &componentJson) override {}
    };

    struct alignas(16) RaytracedBoxComponent : engine::SerializableComponent
    {
        RaytracedBox box;

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
            name = componentJson["name"];
        }
    };
}

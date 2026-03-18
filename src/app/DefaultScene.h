#pragma once

#include "../engine/scene/Scene.h"
#include "scene/Components.h"
#include "../engine/scene/Entity.h"

namespace application
{
    inline constexpr glm::vec3 COLOR_RED(1, 0, 0);
    inline constexpr glm::vec3 COLOR_GREEN(0, 1, 0);
    inline constexpr glm::vec3 COLOR_BLUE(0, 0, 1);
    inline constexpr glm::vec3 COLOR_PURPLE(1, 0, 1);
    inline constexpr glm::vec3 COLOR_WHITE(1);

    /*
     * Temp until scene serialization/deserialization is finished
     */

    inline engine::Entity createSphere(engine::Scene &scene ,const std::string &label, const glm::vec3 &position, const float scale, const glm::vec3 &color)
    {
        const engine::Entity sphere = scene.newEntity(label);

        auto &transform = sphere.addComponent<TransformComponent>();
        transform.position = position;
        transform.scale = glm::vec3(scale);

        auto &material = sphere.addComponent<RaytracedMaterialComponent>();
        material.color = glm::vec4(color, 1);

        sphere.addComponent<RaytracedSphereComponent>();

        return sphere;
    }

    inline engine::Entity createLight(engine::Scene &scene, const std::string label, const glm::vec3 &position, const float scale, const glm::vec3 &emissionColor, const float emissionStrength)
    {
        const engine::Entity light = createSphere(scene, label, position, scale, glm::vec3(0, 0, 0));

        auto &material = light.getComponent<RaytracedMaterialComponent>();
        material.emissionColor = emissionColor;
        material.emissionStrength = emissionStrength;

        return light;
    }

    inline engine::Entity createBox(engine::Scene &scene, const std::string &label, const glm::vec3 &position, const glm::vec3 scale, const glm::vec3 &color)
    {
        const engine::Entity box = scene.newEntity(label);

        auto &transform = box.addComponent<TransformComponent>();
        transform.position = position;
        transform.scale = scale;

        auto &material = box.addComponent<RaytracedMaterialComponent>();
        material.color = color;

        box.addComponent<RaytracedBoxComponent>();

        return box;
    }

    inline void setupDefaultScene(engine::Scene &scene)
    {
        createBox(scene, "Floor Box", glm::vec3(5, -27.20, 0), glm::vec3(50), COLOR_WHITE);
        createBox(scene, "Purple Box", glm::vec3(4.39, -1.67, 7.52), glm::vec3(1), COLOR_PURPLE);

        createSphere(scene, "Blue Sphere", glm::vec3(11, 0, 2), 2.5, COLOR_BLUE);
        createSphere(scene, "Green Sphere", glm::vec3(6, -0.23, 2), 2, COLOR_GREEN);
        createSphere(scene, "Red Sphere", glm::vec3(2, -0.73, 2), 1.5, COLOR_RED);
        createSphere(scene, "White Sphere", glm::vec3(-1.5, -1.21, 2), 1, COLOR_WHITE);

        createLight(scene, "Light", glm::vec3(3.5, 10, 10),4, COLOR_WHITE, 6);
    }
}

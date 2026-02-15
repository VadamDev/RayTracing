#pragma once

#include "../engine/scene/Scene.h"
#include "scene/Components.h"
#include "../engine/scene/Entity.h"

namespace application
{
    /*
     * Temp until scene serialization/deserialization is finished
     */

    static engine::Entity createSphere(engine::Scene &scene ,const std::string &label, const glm::vec3 &position, const float scale, const glm::vec3 &color)
    {
        const engine::Entity sphere = scene.newEntity(label);

        auto &transform = sphere.addComponent<TransformComponent>();
        transform.position = position;
        transform.scale = scale;

        auto &material = sphere.addComponent<RaytracedMaterialComponent>();
        material.color = glm::vec4(color, 1);

        sphere.addComponent<RaytracedSphereComponent>();

        return sphere;
    }

    static engine::Entity createLight(engine::Scene &scene, const std::string label, const glm::vec3 &position, const float scale, const glm::vec3 &emissionColor, const float emissionStrength)
    {
        const engine::Entity light = createSphere(scene, label, position, scale, glm::vec3(0, 0, 0));

        auto &material = light.getComponent<RaytracedMaterialComponent>();
        material.emissionColor = emissionColor;
        material.emissionStrength = emissionStrength;

        return light;
    }

    static void setupDefaultScene(engine::Scene &scene)
    {
        createLight(scene, "Light", glm::vec3(3.5, 10, 10), 4, glm::vec3(1, 1, 1), 8);

        createSphere(scene, "White Sphere", glm::vec3(-1.5, 0, 2), 1, glm::vec3(1, 1, 1));
        createSphere(scene, "Red Sphere", glm::vec3(2, 0, 2), 1.5, glm::vec3(1, 0, 0));
        createSphere(scene, "Green Sphere", glm::vec3(6, 0, 2), 2, glm::vec3(0, 1, 0));
        createSphere(scene, "Blue Sphere", glm::vec3(11, 0, 2), 2.5, glm::vec3(0, 0, 1));

        createSphere(scene, "BIG Sphere", glm::vec3(6, -52, 2), 50, glm::vec3(1, 1, 1));
    }
}

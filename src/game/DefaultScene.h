#pragma once

#include "../engine/scene/Scene.h"
#include "scene/Components.h"
#include "../engine/scene/Entity.h"

namespace application
{
    //Temp until scene serialization/deserialization is finished

    static void createSphere(engine::Scene &scene ,const std::string &label, glm::vec3 position, float scale, glm::vec3 color)
    {
        const engine::Entity sphere = scene.newEntity(label);

        auto &transform = sphere.addComponent<TransformComponent>();
        transform.position = position;
        transform.scale = scale;

        auto &material = sphere.addComponent<RaytracedMaterialComponent>();
        material.color = glm::vec4(color, 1);

        sphere.addComponent<RaytracedSphereComponent>();
    }

    static void setupDefaultScene(engine::Scene &scene)
    {
        createSphere(scene, "White Sphere", glm::vec3(0, 0, 1), 1, glm::vec3(1, 1, 1));
        createSphere(scene, "Red Sphere", glm::vec3(2.5, 0, 1), 1.5, glm::vec3(1, 0, 0));
    }
}

#pragma once

#include "../engine/scene/Scene.h"
#include "scene/Components.h"

namespace application
{
    //Temp until scene serialization/deserialization is finished

    static void setupDefaultScene(engine::Scene &scene)
    {
        scene.newEntity("Empty Entity");

        const auto entity = scene.newEntity("Moveable Entity");
        entity.addComponent<TransformComponent>();
    }
}

#include "Scene.h"

#include "Entity.h"

namespace engine
{
    Entity Scene::newEntity()
    {
        return { registry.create(), this };
    }
}

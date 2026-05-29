#pragma once

#include "Entity.h"

namespace engine
{
    template<typename T>
    struct ComponentSystem
    {
        virtual ~ComponentSystem() = default;

        static bool isAnyPresent(Scene *scene)
        {
            return !scene->registry.storage<T>().empty();
        }
    };
}

#include "Scene.h"

#include "Entity.h"

namespace engine
{
    Entity Scene::newEntity(const std::string &name = "New Entity")
    {
        const Entity entity(registry.create(), this);
        entity.addComponent<TagComponent>(name);

        return entity;
    }

    Entity Scene::copyEntity(const Entity &source, const std::string &name)
    {
        static const entt::id_type TAG_COMPONENT_ID = entt::type_id<TagComponent>().hash();

        const Entity entity = newEntity(name);

        for (auto&& [id, storage] : registry.storage())
        {
            // Ignore TagComponent, since it's added above
            if(id == TAG_COMPONENT_ID)
                continue;

            if(storage.contains(source))
                storage.push(entity, storage.value(source));
        }

        return entity;
    }

    void Scene::destroyEntity(const Entity &entity)
    {
        registry.destroy(entity);
    }
}

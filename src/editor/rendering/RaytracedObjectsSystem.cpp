#include "RaytracedObjectsSystem.h"

#include "RaytraceShader.h"
#include "../model/ModelManager.h"
#include "../../engine/scene/Entity.h"

namespace editor
{
    /*
     * Spheres
     */

    void RaytracedSphereSystem::updateData(engine::Scene *scene)
    {
        /*if (!isAnyPresent(scene))
            return;*/

        std::vector<RaytracedSphere> allSpheres;

        for (auto &entityHandle : scene->registry.view<RaytracedSphereComponent, RaytracedMaterialComponent>())
        {
            const engine::Entity entity = { entityHandle, scene };

            auto &sphere = entity.getComponent<RaytracedSphereComponent>().sphere;
            sphere.material = entity.getComponent<RaytracedMaterialComponent>().material;

            if (entity.hasComponent<TransformComponent>())
            {
                const auto &transform = entity.getComponent<TransformComponent>();
                sphere.position = transform.position;
                sphere.radius = (transform.scale.x + transform.scale.y + transform.scale.z) / 3;
            }

            allSpheres.push_back(sphere);
        }

        shader.updateSpheresBuffer(allSpheres);
    }

    /*
     * Meshes
     */

    void RaytracedMeshSystem::updateData(engine::Scene *scene)
    {
        std::vector<TriangleMeshData> allMeshes;

        for (auto &entityHandle : scene->registry.view<RaytracedMeshComponent, RaytracedMaterialComponent>())
        {
            const engine::Entity entity = { entityHandle, scene };

            auto &raytracedMeshCp = entity.getComponent<RaytracedMeshComponent>();

            auto &mesh = raytracedMeshCp.mesh;
            if (mesh == nullptr)
            {
                mesh = modelManager->load(raytracedMeshCp.name);

                if (!mesh)
                    continue;
            }

            auto transform = glm::identity<glm::mat4>();
            if (entity.hasComponent<TransformComponent>())
                transform = entity.getComponent<TransformComponent>().toTransformMat();

            const TriangleMeshData meshData {
                .rootBVHNodeIndex = mesh->getRootBvhNodeIndex(),

                .localToWorld = transform,
                .worldToLocal = glm::inverse(transform),

                .material = entity.getComponent<RaytracedMaterialComponent>().material
            };

            allMeshes.push_back(meshData);
        }

        shader.updateMeshesBuffer(allMeshes);
    }
}

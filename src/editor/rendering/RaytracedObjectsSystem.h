#pragma once

#include "../../engine/scene/ComponentSystem.hpp"
#include "../scene/Components.h"

namespace editor
{
    class RaytraceShader;
    class ModelManager;

    template<typename T>
    class AbstractRaytracedObjectSystem : public engine::ComponentSystem<T>
    {

    public:
        explicit AbstractRaytracedObjectSystem(RaytraceShader &shader)
            : shader(shader) {}

        virtual void updateData(engine::Scene *scene) = 0;

    protected:
        RaytraceShader &shader;
    };

    /*
     * Spheres
     */

    class RaytracedSphereSystem : public AbstractRaytracedObjectSystem<RaytracedSphere>
    {

    public:
        using AbstractRaytracedObjectSystem::AbstractRaytracedObjectSystem;

        void updateData(engine::Scene *scene) override;
    };

    /*
     * Meshes
     */

    class RaytracedMeshSystem : public AbstractRaytracedObjectSystem<RaytracedMeshComponent>
    {

    public:
        explicit RaytracedMeshSystem(RaytraceShader &shader, ModelManager *modelManager)
            : AbstractRaytracedObjectSystem(shader), modelManager(modelManager) {}

        void updateData(engine::Scene *scene) override;

    private:
        ModelManager *modelManager;
    };
}

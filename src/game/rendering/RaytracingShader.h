#pragma once

#include "Camera.h"
#include "../../engine/graphics/shader/ComputeShader.h"
#include "../../engine/graphics/shader/ShaderStorageBuffer.h"
#include "../model/ModelManager.h"
#include "../scene/Components.h"

namespace application
{
    class RaytracingShader : public engine::ComputeShader
    {
        using UniformAccess = std::unique_ptr<engine::IUniformAccess>;

    public:
        explicit RaytracingShader()
            : ComputeShader("resources/assets/shaders/raytracing_comp.glsl") {}

        void create() override;

        void sendViewParams(Camera *camera) const;
        void setupUniforms() override;

        void updateSpheresBuffer(const std::vector<RaytracedSphereComponent> &spheres);
        void updateBoxesBuffer(const std::vector<RaytracedBoxComponent> &boxes);
        void updateTrianglesBuffer(const std::vector<Triangle> &triangles);
        void updateMeshesBuffer(const std::vector<TriangleMeshData> &meshes);

        UniformAccess frameIndex;
        UniformAccess accumulate;
        UniformAccess maxBounces, raysPerPixel;
        UniformAccess environmentLight;
    private:
        engine::ShaderStorageBuffer<RaytracedSphereComponent> spheresBuffer;
        engine::ShaderStorageBuffer<RaytracedBoxComponent> boxesBuffer;
        engine::ShaderStorageBuffer<Triangle> trianglesBuffer;
        engine::ShaderStorageBuffer<TriangleMeshData> meshesBuffer;

        UniformAccess screenParams, viewParams, cameraPos; //planeWidth, planeHeight, focalLength
        UniformAccess localToWorldMatrix;
    };
}

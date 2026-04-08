#pragma once

#include "Camera.h"
#include "../../engine/graphics/shader/ComputeShader.h"
#include "../../engine/graphics/shader/ShaderStorageBuffer.hpp"
#include "../model/ModelManager.h"
#include "../scene/RaytracedObjects.h"

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

        void updateSpheresBuffer(const std::vector<RaytracedSphere> &spheres);
        void updateBoxesBuffer(const std::vector<RaytracedBox> &boxes);
        void updateTrianglesBuffer(const std::vector<RaytracedTriangle> &triangles);
        void updateMeshesBuffer(const std::vector<TriangleMeshData> &meshes);

        UniformAccess frameIndex;
        UniformAccess accumulate;
        UniformAccess maxBounces, raysPerPixel;
        UniformAccess environmentLight;
        UniformAccess divergeStrength;
        UniformAccess defocusStrength;
    private:
        engine::ShaderStorageBuffer<RaytracedSphere> spheresBuffer;
        engine::ShaderStorageBuffer<RaytracedBox> boxesBuffer;
        engine::ShaderStorageBuffer<RaytracedTriangle> trianglesBuffer;
        engine::ShaderStorageBuffer<TriangleMeshData> meshesBuffer;

        UniformAccess screenParams, viewParams; // planeWidth, planeHeight, focalLength
        UniformAccess localToWorldMatrix;
    };
}

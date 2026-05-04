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
        void updateMeshDataBuffers(const std::vector<RaytracedTriangle> &triangles, const std::vector<RaytracedBVHNode> &bvhNodes);
        void updateMeshesBuffer(const std::vector<TriangleMeshData> &meshes);

        void unbindBuffers();

        UniformAccess frameIndex;
        UniformAccess drawDebugMode;
        UniformAccess statsThresholds;
        UniformAccess accumulate;
        UniformAccess maxBounces, raysPerPixel;
        UniformAccess environmentLight;
        UniformAccess divergeStrength;
        UniformAccess defocusStrength;
    private:
        engine::ShaderStorageBuffer<RaytracedSphere> spheresBuffer;
        engine::ShaderStorageBuffer<RaytracedTriangle> trianglesBuffer;
        engine::ShaderStorageBuffer<RaytracedBVHNode> bvhNodesBuffer;
        engine::ShaderStorageBuffer<TriangleMeshData> meshesBuffer;

        UniformAccess viewParams; // planeWidth, planeHeight, focalLength
        UniformAccess localToWorldMatrix;
    };
}

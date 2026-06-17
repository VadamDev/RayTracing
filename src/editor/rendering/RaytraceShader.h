#pragma once

#include "../../engine/graphics/shaders/ComputeShader.h"
#include "../scene/RaytracedObjects.h"
#include "../../engine/graphics/shaders/ShaderStorageBuffer.hpp"

namespace editor
{
    class CameraSystem;

    struct RaytracerSettings
    {
        bool accumulate = true;
        int maxBounces = 4;
        int raysPerPixel = 1;
        bool environmentalLight = true;
        float divergeStrength = 0.5f;
        float defocusStrength = 0;
    };

    const std::vector DRAW_MODE_NAMES = { "Default", "Box Tests", "Tri Tests", "Box + Tri Tests", "Normals" };
    enum class DrawMode : int
    {
        DEFAULT = 0,
        BOX_TESTS = 1,
        TRI_TESTS = 2,
        BOX_AND_TRI_TESTS = 3,
        NORMALS = 4
    };

    struct DebugSettings
    {
        DrawMode drawMode = DrawMode::DEFAULT;
        glm::vec2 statsThreshold = { 1, 1 };
    };

    class RaytraceShader : public engine::ComputeShader
    {

    public:
        RaytraceShader()
            : ComputeShader("resources/shaders/raytracing_comp.glsl") {}

        void create() override;

        void updateFrameIndex(unsigned int frameIndex);
        void updateViewData(CameraSystem *cameraSystem, float aspectRatio) const;
        void updateRaytracerSettings(const RaytracerSettings &settings) const;
        void updateRaytracerDebugSettings(const DebugSettings &debugSettings) const;

        void updateSpheresBuffer(const std::vector<RaytracedSphere> &spheres);
        void updateMeshDataBuffers(const std::vector<RaytracedTriangle> &triangles, const std::vector<RaytracedBVHNode> &nodes);
        void updateMeshesBuffer(const std::vector<TriangleMeshData> &meshes);
        void unbindBuffers();

    protected:
        void setupUniforms() override;

    private:
        using UniformAccess = std::unique_ptr<engine::IUniformAccess>;

        // Buffers
        engine::ShaderStorageBuffer<RaytracedSphere> spheres;
        engine::ShaderStorageBuffer<RaytracedTriangle> triangles;
        engine::ShaderStorageBuffer<RaytracedBVHNode> nodes;
        engine::ShaderStorageBuffer<TriangleMeshData> meshes;

        // Global
        UniformAccess frameIndex;

        // Camera
        UniformAccess viewParams, localToWorldMat;

        // Raytracer Settings
        UniformAccess accumulate;
        UniformAccess maxBounces;
        UniformAccess raysPerPixel;
        UniformAccess environmentLight;
        UniformAccess divergeStrength;
        UniformAccess defocusStrength;

        // Debug
        UniformAccess drawDebugMode;
        UniformAccess statsThresholds;
    };
}

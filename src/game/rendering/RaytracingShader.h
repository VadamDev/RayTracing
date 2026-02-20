#pragma once

#include "Camera.h"
#include "../../engine/graphics/shader/ComputeShader.h"
#include "../../engine/graphics/shader/ShaderStorageBuffer.h"
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

        UniformAccess frameIndex;
        UniformAccess accumulate;
        UniformAccess maxBounces, raysPerPixel;
    private:
        engine::ShaderStorageBuffer<RaytracedSphereComponent> spheresBuffer;
        engine::ShaderStorageBuffer<RaytracedBoxComponent> boxesBuffer;

        UniformAccess screenParams, viewParams, cameraPos; //planeWidth, planeHeight, focalLength
        UniformAccess localToWorldMatrix;
    };
}

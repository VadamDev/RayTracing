#pragma once

#include "../../engine/graphics/shader/ShaderProgram.h"
#include "Camera.h"

namespace application
{
    class TracingShader : public engine::ShaderProgram
    {
        using UniformAccess = std::unique_ptr<engine::IUniformAccess>;

    public:
        explicit TracingShader()
            : ShaderProgram("resources/assets/shaders/vert.glsl", "resources/assets/shaders/frag.glsl") {}

        void sendViewParams(Camera *camera) const;
        void setupUniforms() override;

        UniformAccess currentFrameTime;
        UniformAccess numSpheres, maxBounces, raysPerPixel;
    private:
        UniformAccess screenParams, viewParams, cameraPos; //planeWidth, planeHeight, focalLength

        UniformAccess localToWorldMatrix;
    };
}

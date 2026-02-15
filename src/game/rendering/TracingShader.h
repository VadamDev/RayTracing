#pragma once

#include "../../engine/graphics/shader/ShaderProgram.h"
#include "Camera.h"

namespace application
{
    class TracingShader : public engine::ShaderProgram
    {

        using UniformAccess = std::unique_ptr<engine::IUniformAccess>;
    public:
        TracingShader() : ShaderProgram("resources/assets/shaders/vert.glsl", "resources/assets/shaders/frag.glsl") {}

        void sendViewParams(Camera *camera) const;

        void setupUniforms() override;

        UniformAccess numSpheres;
    private:
        UniformAccess screenParams, viewParams, cameraPos; //planeWidth, planeHeight, focalLength

        UniformAccess localToWorldMatrix;
    };
}

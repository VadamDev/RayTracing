#pragma once

#include "../../engine/graphics/shader/ShaderProgram.h"
#include "Camera.h"

namespace application
{
    class TracingShader : public engine::ShaderProgram
    {

    public:
        TracingShader() : ShaderProgram("resources/assets/shaders/vert.glsl", "resources/assets/shaders/frag.glsl") {}

        void sendViewParams(Camera *camera) const;

        void setupUniforms() override;
    private:
        using UniformAccess = std::unique_ptr<engine::IUniformAccess>;

        UniformAccess screenParams;
        UniformAccess viewParams; //planeWidth, planeHeight, focalLength
        UniformAccess cameraPos;

        UniformAccess localToWorldMatrix;
    };
}

#pragma once

#include "../../../engine/graphics/shader/ShaderProgram.h"
#include "../camera/Camera.h"

namespace game
{
    class TracingShader : public engine::ShaderProgram
    {

    public:
        TracingShader() : ShaderProgram("resources/assets/shaders/vert.glsl", "resources/assets/shaders/frag.glsl") {}

        void sendViewParams(Camera *camera) const;

        void setupUniforms() override;
    private:
        using UniformAccess = std::unique_ptr<engine::UniformAccess>;

        UniformAccess viewParams; //planeWidth, planeHeight, focalLength
        UniformAccess cameraPos;

        UniformAccess localToWorldMatrix;
    };
}

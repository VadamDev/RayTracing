#pragma once

#include <memory>

#include "../engine/graphics/shader/ShaderProgram.h"

namespace game {
    class TestShader : public engine::ShaderProgram {

    public:
        TestShader(): ShaderProgram("resources/assets/shaders/vert.glsl", "resources/assets/shaders/frag.glsl") {}

        std::unique_ptr<engine::UniformAccess> frameTime;

    protected:
        void setupUniforms() override;
    };
}

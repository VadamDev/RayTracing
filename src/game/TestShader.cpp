#include "TestShader.h"

namespace game {
    void TestShader::setupUniforms()
    {
        const engine::UniformAccess testUniform = accessUniform("test");
        testUniform.set1f(1.0);
    }
}
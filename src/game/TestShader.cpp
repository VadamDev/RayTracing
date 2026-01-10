#include "TestShader.h"

namespace game {
    void TestShader::setupUniforms()
    {
        frameTime = accessUniform("time");
    }
}

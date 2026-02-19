#include "ComputeShader.h"

#include "exceptions/ShaderException.h"
#include "../../utils/FileReader.h"

namespace engine
{
    void ComputeShader::create()
    {
        const std::string shaderSource = utils::readFile(shaderPath);
        if (shaderSource.empty())
            throw exceptions::ProgramException("Shader source code is empty", 0);

        const unsigned int shaderId = createShader(shaderSource.c_str(), GL_COMPUTE_SHADER);

        programId = glCreateProgram();

        glAttachShader(programId, shaderId);
        glLinkProgram(programId);

        glDeleteShader(shaderId);

        assertProgramLinked();
        setupUniforms();
    }

    void ComputeShader::dispatchCompute(const unsigned int numGroupsX, const unsigned int numGroupsY, const unsigned int numGroupsZ, const unsigned int memoryBarrier)
    {
        glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
        glMemoryBarrier(memoryBarrier);
    }

    void ComputeShader::waitCompletion()
    {
        glFinish();
    }
}

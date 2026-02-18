#include "AbstractShader.h"

#include <spdlog/spdlog.h>
#include <glad/glad.h>

#include "uniforms/UniformAccess.h"
#include "exceptions/ShaderException.h"

namespace engine
{
    AbstractShader::~AbstractShader()
    {
        if (bBound)
            unbind();

        if (programId != 0)
            glDeleteProgram(programId);
    }

    void AbstractShader::bind()
    {
        glUseProgram(programId);
        bBound = true;
    }

    void AbstractShader::unbind()
    {
        glUseProgram(0);
        bBound = false;
    }

    std::unique_ptr<IUniformAccess> AbstractShader::accessUniform(const std::string &name) const
    {
        const int location = glGetUniformLocation(programId, name.c_str());
        if (location < 0)
        {
            spdlog::warn("Failed to retrieve uniform location for {}", name);
            return std::make_unique<NoOpUniformAccess>();
        }

        return std::make_unique<UniformAccess>(location);
    }

    unsigned int AbstractShader::createShader(const char *source, const unsigned int type)
    {
        const unsigned int shaderId = glCreateShader(type);
        if (shaderId == 0)
            throw exceptions::ShaderException("Failed to create shader", type);

        glShaderSource(shaderId, 1, &source, nullptr);
        glCompileShader(shaderId);

        int success;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[1024];
            glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);

            glDeleteShader(shaderId);
            throw exceptions::ShaderCompileException(infoLog, type);
        }

        return shaderId;
    }

    void AbstractShader::assertProgramLinked() const
    {
        int success;
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success)
        {
            char log[1024];
            glGetProgramInfoLog(programId, 1024, nullptr, log);

            glDeleteProgram(programId);
            throw exceptions::ProgramLinkException(log, programId);
        }
    }
}

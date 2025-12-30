#include "ShaderProgram.h"

#include "../../utils/FileReader.h"
#include "exceptions/ShaderException.h"

namespace engine {
    ShaderProgram::~ShaderProgram()
    {
        unbind();

        if (programId != 0)
            glDeleteProgram(programId);
    }

    void ShaderProgram::create()
    {
        const std::string vertexSource = utils::readFile(vertexPath);
        const std::string fragmentSource = utils::readFile(fragmentPath);

        if (vertexSource.empty() || fragmentSource.empty())
            throw exceptions::ProgramException("Shader source code is empty", 0);

        const unsigned int vertexShader = createShader(vertexSource.c_str(), GL_VERTEX_SHADER);
        const unsigned int fragmentShader = createShader(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

        programId = glCreateProgram();

        glAttachShader(programId, vertexShader);
        glAttachShader(programId, fragmentShader);
        glLinkProgram(programId);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        int success;
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success)
        {
            char log[1024];
            glGetProgramInfoLog(programId, 1024, nullptr, log);

            glDeleteProgram(programId);

            throw exceptions::ProgramLinkException(log, programId);
        }

        setupUniforms();
    }

    void ShaderProgram::bind() const
    {
        glUseProgram(programId);
    }

    void ShaderProgram::unbind()
    {
        glUseProgram(0);
    }

    UniformAccess ShaderProgram::accessUniform(const std::string &name) const
    {
        const int location = glGetUniformLocation(programId, name.c_str());
        if (location < 0)
            throw exceptions::UniformException(std::format("Failed to retrieve uniform location for {}", name), name, location);

        return UniformAccess(location);
    }

    /*
       Static Utils
     */

    unsigned int ShaderProgram::createShader(const char *source, const unsigned int type)
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
}

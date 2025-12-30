#pragma once

#include <glad/glad.h>
#include <stdexcept>

namespace engine::exceptions
{
    /*
       Utils
     */

    static std::string formatShaderType(const unsigned int type)
    {
        switch (type)
        {
            case GL_VERTEX_SHADER:
                return "VERTEX_SHADER";
            case GL_FRAGMENT_SHADER:
                return "FRAGMENT_SHADER";
            default:
                return "UNKNOWN";
        }
    }

    /*
       Shader
     */

    class ShaderException : public std::runtime_error
    {

    public:
        explicit ShaderException(const std::string &message, const unsigned int type)
            : ShaderException(message.c_str(), type) {}

        explicit ShaderException(const char *message, const unsigned int type)
            : runtime_error(message), type(type) {}

        unsigned int getType() const { return type; }
        std::string formattedType() const { return formatShaderType(type); }

    private:
        const unsigned int type;
    };

    class ShaderCompileException final : public ShaderException
    {

    public:
        using ShaderException::ShaderException;
    };

    /*
       Program
     */

    class ProgramException : public std::runtime_error
    {

    public:
        explicit ProgramException(const std::string &message, const unsigned int programId)
            : ProgramException(message.c_str(), programId) {}

        explicit ProgramException(const char *message, const unsigned int programId)
            : runtime_error(message), programId(programId) {}

        unsigned int getProgramId() const { return programId; }

    private:
        unsigned int programId;
    };

    class ProgramLinkException : public ProgramException
    {

    public:
        using ProgramException::ProgramException;
    };

    /*
       Uniform
     */

    class UniformException : public std::runtime_error
    {

    public:
        explicit UniformException(const std::string &message, const std::string &name, const int position)
            : UniformException(message.c_str(), name, position) {}

        explicit UniformException(const char *message, const std::string &name, const int position)
            : runtime_error(message), name(name), position(position) {}

        std::string getName() const { return name; }
        int getPosition() const { return position; }
    private:
        const std::string &name;
        const int position;
    };
}

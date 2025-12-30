#pragma once

#include <string>

#include "UniformAccess.h"
#include "exceptions/ShaderException.h"

namespace engine {
    class ShaderProgram {

    public:
        static unsigned int createShader(const char *source, unsigned int type);

        explicit ShaderProgram(std::string vertexPath, std::string fragmentPath)
            : vertexPath(std::move(vertexPath)), fragmentPath(std::move(fragmentPath)) {}
        virtual ~ShaderProgram();

        void create();

        void bind() const;
        static void unbind();

        UniformAccess accessUniform(const std::string &name) const;

    private:
        std::string vertexPath, fragmentPath;
        unsigned int programId = 0;

    protected:
        virtual void setupUniforms() = 0;
    };
}

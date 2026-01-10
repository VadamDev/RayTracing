#pragma once

#include <memory>
#include <string>

#include "UniformAccess.h"
#include "exceptions/ShaderException.h"

namespace engine {
    class ShaderProgram {

    public:
        explicit ShaderProgram(std::string vertexPath, std::string fragmentPath)
            : vertexPath(std::move(vertexPath)), fragmentPath(std::move(fragmentPath)) {}
        virtual ~ShaderProgram();

        void create();

        void bind() const;
        static void unbind();

        std::unique_ptr<UniformAccess> accessUniform(const std::string &name) const;

        static unsigned int createShader(const char *source, unsigned int type);

    private:
        const std::string vertexPath, fragmentPath;
        unsigned int programId = 0;

    protected:
        virtual void setupUniforms() = 0;
    };
}

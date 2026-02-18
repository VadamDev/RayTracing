#pragma once

#include "AbstractShader.h"

namespace engine {
    class ShaderProgram : public AbstractShader
    {

    public:
        explicit ShaderProgram(std::string vertexPath, std::string fragmentPath)
            : vertexPath(std::move(vertexPath)), fragmentPath(std::move(fragmentPath)) {}

        void create() override;

    private:
        const std::string vertexPath, fragmentPath;
    };
}

#pragma once

#include "AbstractShader.h"

namespace engine
{
    class ComputeShader : public AbstractShader
    {

    public:
        explicit ComputeShader(std::string shaderPath)
            : shaderPath(std::move(shaderPath)) {}

        void create() override;

        static void dispatchCompute(unsigned int numGroupsX, unsigned int numGroupsY, unsigned int numGroupsZ, unsigned int memoryBarrier);
        static void waitCompletion();

    private:
        const std::string shaderPath;
    };
}

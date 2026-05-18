#pragma once

#include <memory>
#include <string>

#include "uniforms/IUniformAccess.h"

namespace engine
{
    class AbstractShader
    {

    public:
        virtual ~AbstractShader();

        virtual void create() = 0;

        void bind();
        void unbind();

        std::unique_ptr<IUniformAccess> accessUniform(const std::string &name) const;

        bool isBound() const { return bBound; }

        static unsigned int createShader(const char *source, unsigned int type);

    protected:
        unsigned int programId = 0;
        bool bBound = false;

        virtual void setupUniforms() = 0;

        void assertProgramLinked() const;
    };
}

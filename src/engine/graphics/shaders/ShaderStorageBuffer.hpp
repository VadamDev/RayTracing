#pragma once

#include <glad/glad.h>
#include <vector>
#include <stdexcept>

namespace engine
{
    template<typename T>
    class ShaderStorageBuffer
    {

    public:
        ~ShaderStorageBuffer()
        {
            if (bound)
                unbind();

            if (ssboHandle != 0)
                glDeleteBuffers(1, &ssboHandle);
        }

        void create()
        {
            glGenBuffers(1, &ssboHandle);
        }

        void bind()
        {
            assertCreated();

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboHandle);
            bound = true;
        }

        void unbind()
        {
            assertCreated();

            if (!bound)
                return;

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
            bound = false;
        }

        void update(const std::vector<T> &data, const unsigned int binding, const unsigned int usage)
        {
            assertCreated();

            currentSize = data.size();

            bind();

            glNamedBufferData(ssboHandle, sizeof(T) * currentSize, data.data(), usage);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, ssboHandle);
        }

        bool isBound() const { return bound; }
        size_t size() const { return currentSize; }

    private:
        unsigned int ssboHandle = 0;
        bool bound = false;

        size_t currentSize = 0;

        void assertCreated() const
        {
            if (ssboHandle == 0)
                throw std::runtime_error("The SSBO hasn't been created");
        }
    };
}
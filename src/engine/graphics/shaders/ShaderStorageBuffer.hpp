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

            if (ssbo != 0)
                glDeleteBuffers(1, &ssbo);
        }

        void create()
        {
            glGenBuffers(1, &ssbo);
        }

        void bind()
        {
            if (ssbo == 0)
                throw std::runtime_error("Failed to bind SSBO: The SSBO hasn't been created");

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
            bound = true;
        }

        void unbind()
        {
            if (ssbo == 0)
                throw std::runtime_error("Failed to unbind SSBO: The SSBO hasn't been created");

            if (!bound)
                return;

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
            bound = false;
        }

        void update(const std::vector<T> &data, const unsigned int binding, const unsigned int usage)
        {
            if (ssbo == 0)
                throw std::runtime_error("Failed to update SSBO: The SSBO hasn't been created");

            currentSize = data.size();

            bind();

            glNamedBufferData(ssbo, sizeof(T) * currentSize, data.data(), usage);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, ssbo);
        }

        bool isBound() const { return bound; }
        size_t size() const { return currentSize; }

    private:
        unsigned int ssbo = 0;
        bool bound = false;

        size_t currentSize = 0;
    };
}
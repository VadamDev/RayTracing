#pragma once

#include <glad/glad.h>
#include <vector>

namespace engine
{
    template<typename T>
    class ShaderStorageBuffer
    {

    public:
        ~ShaderStorageBuffer()
        {
            if (bBound)
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
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
            bBound = true;
        }

        void unbind()
        {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
            bBound = false;
        }

        void update(const std::vector<T> &data, const unsigned int binding, const unsigned int usage)
        {
            currentSize = data.size();

            bind();

            glNamedBufferData(ssbo, sizeof(T) * currentSize, data.data(), usage);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, ssbo);

            unbind();
        }

        bool isBound() const { return bBound; }
        size_t size() const { return currentSize; }

    private:
        unsigned int ssbo = 0;
        bool bBound = false;

        size_t currentSize = 0;
    };
}

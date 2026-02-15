#include "SpheresShaderBuffer.h"

#include "glad/glad.h"

namespace application
{
    SpheresShaderBuffer::~SpheresShaderBuffer()
    {
        glDeleteBuffers(1, &ssbo);
    }

    void SpheresShaderBuffer::create()
    {
        glGenBuffers(1, &ssbo);
    }

    void SpheresShaderBuffer::update(const std::vector<RaytracedSphereComponent> &spheres)
    {
        currentSize = spheres.size();

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

        glNamedBufferData(ssbo, sizeof(RaytracedSphereComponent) * currentSize, spheres.data(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
}

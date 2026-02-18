#include "UniformAccess.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace engine {
    void UniformAccess::set1f(const float value) const
    {
        glUniform1f(location, value);
    }

    void UniformAccess::set2f(const float x, const float y) const
    {
        glUniform2f(location, x, y);
    }

    void UniformAccess::set2f(const glm::vec2 &vector) const
    {
        glUniform2f(location, vector.x, vector.y);
    }

    void UniformAccess::set3f(const float x, const float y, const float z) const
    {
        glUniform3f(location, x, y, z);
    }

    void UniformAccess::set3f(const glm::vec3 &vector) const
    {
        glUniform3f(location, vector.x, vector.y, vector.z);
    }

    void UniformAccess::setMatrix4f(const glm::mat4 &matrix) const
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void UniformAccess::set1i(const int value) const
    {
        glUniform1i(location, value);
    }

    void UniformAccess::set2i(const int x, const int y) const
    {
        glUniform2i(location, x, y);
    }

    void UniformAccess::set2i(const glm::ivec2 &vector) const
    {
        glUniform2i(location, vector.x, vector.y);
    }

    void UniformAccess::set3i(const int x, const int y, const int z) const
    {
        glUniform3i(location, x, y, z);
    }

    void UniformAccess::set3i(const glm::ivec3 &vector) const
    {
        glUniform3i(location, vector.x, vector.y, vector.z);
    }

    void UniformAccess::setBool(const bool b) const
    {
        glUniform1i(location, b ? 1 : 0);
    }
}

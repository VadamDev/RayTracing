#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace engine {
    class UniformAccess {

    public:
        explicit UniformAccess(const int location)
            : location(location) {}

        /*
           Float
         */

        void set1f(float value) const;

        void set2f(float x, float y) const;
        void set2f(const glm::vec2 &vector) const;

        void set3f(float x, float y, float z) const;
        void set3f(const glm::vec3 &vector) const;

        void setMatrix4f(const glm::mat4 &matrix) const;

        /*
           Int
         */

        void set1i(int value) const;

        void set2i(int x, int y) const;
        void set2i(const glm::ivec2 &vector) const;

        void set3i(int x, int y, int z) const;
        void set3i(const glm::ivec3 &vector) const;

        /*
           Boolean
         */

        void setBool(bool b) const;

    private:
        int location;
    };
}

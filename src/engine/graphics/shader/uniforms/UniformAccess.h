#pragma once

#include "IUniformAccess.h"

namespace engine {
    class UniformAccess : public IUniformAccess
    {

    public:
        explicit UniformAccess(const int location)
            : location(location) {}

        /*
           Float
         */

        void set1f(float value) const override;

        void set2f(float x, float y) const override;
        void set2f(const glm::vec2 &vector) const override;

        void set3f(float x, float y, float z) const override;
        void set3f(const glm::vec3 &vector) const override;

        void setMatrix4f(const glm::mat4 &matrix) const override;

        /*
           Int
         */

        void set1i(int value) const override;

        void set2i(int x, int y) const override;
        void set2i(const glm::ivec2 &vector) const override;

        void set3i(int x, int y, int z) const override;
        void set3i(const glm::ivec3 &vector) const override;

        /*
           Boolean
         */

        void setBool(bool b) const override;

    private:
        int location;
    };
}

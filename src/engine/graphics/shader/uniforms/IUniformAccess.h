#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace engine
{
    class IUniformAccess
    {

    public:
        virtual ~IUniformAccess() = default;

        /*
           Float
         */

        virtual void set1f(float value) const = 0;

        virtual void set2f(float x, float y) const = 0;
        virtual void set2f(const glm::vec2 &vector) const = 0;

        virtual void set3f(float x, float y, float z) const = 0;
        virtual void set3f(const glm::vec3 &vector) const = 0;

        virtual void setMatrix4f(const glm::mat4 &matrix) const = 0;

        /*
           Int
         */

        virtual void set1i(int value) const = 0;

        virtual void set2i(int x, int y) const = 0;
        virtual void set2i(const glm::ivec2 &vector) const = 0;

        virtual void set3i(int x, int y, int z) const = 0;
        virtual void set3i(const glm::ivec3 &vector) const = 0;

        /*
           Boolean
         */

        virtual void setBool(bool b) const = 0;
    };

    //Since GLSL compiler scrap every unused variable, we just use a no operation placeholder.
    //It might be smarter to only return this class on test builds.
    class NoOpUniformAccess : public IUniformAccess
    {

    public:
        void set1f(float value) const override {}

        void set2f(float x, float y) const override {}
        void set2f(const glm::vec2 &vector) const override {}

        void set3f(const glm::vec3 &vector) const override {}
        void set3f(float x, float y, float z) const override {}

        void setMatrix4f(const glm::mat4 &matrix) const override {}

        void set1i(int value) const override {}

        void set2i(int x, int y) const override {}
        void set2i(const glm::ivec2 &vector) const override {}

        void set3i(int x, int y, int z) const override {}
        void set3i(const glm::ivec3 &vector) const override {}

        void setBool(bool b) const override {}
    };
}
#pragma once

#include <vector>
#include <string>

namespace engine
{
    class Cubemap
    {

    public:
        explicit Cubemap(int width, int height, const std::vector<std::string> &faces);
        ~Cubemap();

        void create();

        void bind() const;
        static void unbind();

    private:
        int width, height;
        std::vector<std::string> faces;

        unsigned int textureHandle = 0;
    };
}

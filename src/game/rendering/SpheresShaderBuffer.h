#pragma once

#include "../scene/Components.h"

namespace application
{
    class SpheresShaderBuffer
    {

    public:
        ~SpheresShaderBuffer();

        void create();

        void update(const std::vector<RaytracedSphereComponent> &spheres);
        size_t size() const { return currentSize; }

    private:
        unsigned int ssbo = 0;
        size_t currentSize = 0;
    };
}
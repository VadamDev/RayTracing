#pragma once

namespace engine
{
    class Renderable
    {

    public:
        virtual ~Renderable() = default;

        virtual void render() = 0;
    };
}

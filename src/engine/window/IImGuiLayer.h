#pragma once

namespace engine
{
    class IImGuiLayer
    {

    public:
        virtual ~IImGuiLayer() = default;

        virtual void draw() = 0;
    };
}

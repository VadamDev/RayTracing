#pragma once

namespace engine
{
    class IImGuiWindow
    {

    public:
        virtual ~IImGuiWindow() = default;

        virtual void draw() = 0;
        virtual bool shouldDraw() = 0;
    };
}

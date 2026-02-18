#pragma once

namespace engine
{
    class IImGuiWindow
    {

    public:
        virtual ~IImGuiWindow() = default;

        virtual void draw() = 0;
    };
}

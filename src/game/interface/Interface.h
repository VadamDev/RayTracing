#pragma once

#include "../../engine/window/imgui/IImGuiWindow.h"

namespace application
{
    class Interface : public engine::IImGuiWindow
    {

    public:
        Interface();

        void draw() override;
        bool shouldDraw() override
        {
            return true;
        }
    };
}
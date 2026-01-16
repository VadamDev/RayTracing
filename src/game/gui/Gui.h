#pragma once

#include "../../engine/window/imgui/IImGuiWindow.h"
#include "../../engine/AbstractGame.h"
#include "../../engine/clock/FixedStepClock.h"
#include "../rendering/Renderer.h"

namespace game
{
    class Gui : public engine::IImGuiWindow
    {

    public:
        explicit Gui(const engine::AbstractGame *game, const Renderer *renderer)
            : window(game->getWindow()), clock(dynamic_cast<engine::FixedStepClock*>(game->getClock())), renderer(renderer) {}

        void draw() override;

        bool shouldDraw() override
        {
            return true;
        }

    private:
        engine::Window &window;
        engine::FixedStepClock *clock;

        const Renderer *renderer;

        bool bUnlimitedFps = false;
    };
}

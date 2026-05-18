#pragma once
#include "UIPanel.h"
#include "../../engine/clock/SimpleClock.h"

namespace editor
{
    class TestPanel : public UIPanel
    {
    public:
        explicit TestPanel(engine::SimpleClock *clock)
            : UIPanel("Test Panel"), clock(clock) {}

        void draw() override;

    private:
        engine::SimpleClock *clock;
    };
} // editor

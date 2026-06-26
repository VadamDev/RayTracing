#pragma once

#include "../UIPanel.h"

namespace engine
{
    class SimpleClock;
}

namespace editor
{
    class RenderManager;

    class RenderStatusPanel : public UIPanel
    {

    public:
        explicit RenderStatusPanel(engine::SimpleClock *clock, RenderManager *renderManager)
            : UIPanel("Status"), clock(clock), renderManager(renderManager) {}

        void draw(float deltaTime) override;
    private:
        engine::SimpleClock *clock;

        RenderManager *renderManager;
    };
}

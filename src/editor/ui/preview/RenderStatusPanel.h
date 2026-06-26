#pragma once
#include "../UIPanel.h"

namespace editor
{
    class RenderManager;

    class RenderStatusPanel : public UIPanel
    {

    public:
        explicit RenderStatusPanel(RenderManager *renderManager)
            : UIPanel("Status"), renderManager(renderManager) {}

        void draw(float deltaTime) override;
    private:
        RenderManager *renderManager;
    };
}

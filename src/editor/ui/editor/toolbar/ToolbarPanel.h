#pragma once

#include "../../UIPanel.h"

namespace editor
{
    class SceneHandler;
    class RenderManager;

    class ToolbarPanel : public UIPanel
    {

    public:
        explicit ToolbarPanel(SceneHandler *sceneHandler, RenderManager *renderManager)
            : UIPanel("Toolbar"), sceneHandler(sceneHandler), renderManager(renderManager) {}

        void draw(float deltaTime) override;

    private:
        SceneHandler *sceneHandler;
        RenderManager *renderManager;
    };
}

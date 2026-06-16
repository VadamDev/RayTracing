#pragma once

#include "../UIPanel.h"

namespace editor
{
    class SceneHandler;

    class ToolbarPanel : public UIPanel
    {

    public:
        explicit ToolbarPanel(SceneHandler *sceneHandler)
            : UIPanel("Toolbar"), sceneHandler(sceneHandler) {}

        void draw(float deltaTime) override;

    private:
        SceneHandler *sceneHandler;
    };
}

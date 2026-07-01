#pragma once

#include "../../UIPanel.h"

namespace engine
{
    class Messenger;
}

namespace editor
{
    class SceneHandler;
    class RenderManager;
    class RenderSettingsPanel;

    class ToolbarPanel : public UIPanel
    {

    public:
        explicit ToolbarPanel(engine::Messenger &globalMessenger, SceneHandler *sceneHandler, RenderManager *renderManager, RenderSettingsPanel *renderSettingsPanel)
            : UIPanel("Toolbar"), globalMessenger(globalMessenger), sceneHandler(sceneHandler), renderManager(renderManager), renderSettingsPanel(renderSettingsPanel) {}

        void draw(float deltaTime) override;

    private:
        engine::Messenger &globalMessenger;

        SceneHandler *sceneHandler;
        RenderManager *renderManager;
        RenderSettingsPanel *renderSettingsPanel;
    };
}

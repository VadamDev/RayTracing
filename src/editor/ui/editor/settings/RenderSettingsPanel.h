#pragma once

#include "../../UIPanel.h"
#include "../Menu.h"
#include "../../../rendering/RenderManager.h"

namespace editor
{
    class RenderingCanvas;

    class RenderSettingsPanel : public UIPanel
    {

    public:
        explicit RenderSettingsPanel(engine::Messenger &globalMessenger, RenderingCanvas *canvas)
            : UIPanel(menu::RENDER_SETTINGS_MENU), globalMessenger(globalMessenger), canvas(canvas) {}

        void draw(float deltaTime) override;

        RenderOptions getSelectedRenderOptions() const { return renderOptions; }

    private:
        engine::Messenger &globalMessenger;

        RenderingCanvas *canvas;
        RenderOptions renderOptions;
    };
}

#pragma once

#include "../../UIPanel.h"

namespace engine
{
    class Window;
    class SimpleClock;
    class Messenger;
}

namespace editor
{
    class RenderingCanvas;
    class RaytraceComputeLayer;
    class ViewportPanel;

    class SettingsPanel : public UIPanel
    {

    public:
        explicit SettingsPanel(engine::Window &window, engine::SimpleClock *clock, engine::Messenger &globalMessenger, RaytraceComputeLayer *raytraceComputeLayer, RenderingCanvas *canvas, ViewportPanel *viewportPanel)
            : UIPanel("Settings"), window(window), clock(clock), globalMessenger(globalMessenger), raytraceComputeLayer(raytraceComputeLayer), canvas(canvas), viewportPanel(viewportPanel) {}

        void draw(float deltaTime) override;

    private:
        engine::Window &window;
        engine::SimpleClock *clock;
        engine::Messenger &globalMessenger;

        RaytraceComputeLayer *raytraceComputeLayer;
        RenderingCanvas *canvas;
        ViewportPanel *viewportPanel;

        float lastSpentTimeMs = 0, spentTimeAvgMs = 0;
        void updateData();
    };
}

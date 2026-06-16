#pragma once

#include "../UIPanel.h"

namespace engine
{
    class SimpleClock;
}

namespace editor
{
    class RenderingCanvas;
    class RaytraceComputeLayer;

    class SettingsPanel : public UIPanel
    {

    public:
        explicit SettingsPanel(engine::SimpleClock *clock, RaytraceComputeLayer *raytraceComputeLayer, RenderingCanvas *canvas)
            : UIPanel("Settings"), clock(clock), raytraceComputeLayer(raytraceComputeLayer), canvas(canvas) {}

        void draw(float deltaTime) override;

    private:
        engine::SimpleClock *clock;

        RaytraceComputeLayer *raytraceComputeLayer;
        RenderingCanvas *canvas;

        float lastSpentTimeMs = 0, spentTimeAvgMs = 0;
        void updateData();
    };
}

#include "SettingsPanel.h"

#include <imgui.h>
#include "../../../../engine/clock/SimpleClock.h"
#include "../../ImGuiUtils.hpp"
#include "../../../layers/RaytraceComputeLayer.h"
#include "../../../rendering/RenderingCanvas.h"
#include "../../../rendering/RenderingEvents.h"
#include "../../../../engine/messenger/Messenger.hpp"

namespace editor
{
    static constexpr float COLUMN_WIDTH = 150.0f;

    void SettingsPanel::draw(const float deltaTime)
    {
        static float timer = 0;
        if ((timer += deltaTime) > 1)
        {
            updateData();
            timer = 0;
        }

        ImGui::Begin(getName(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing);

        if (ImGui::CollapsingHeader("Stats", ImGuiTreeNodeFlags_DefaultOpen))
        {
            static bool vsync = false;

            ImGui::Text(std::format("FPS: {} ({:.3f}ms) | Avg: {:.3f}ms", clock->getFPS(), lastSpentTimeMs, spentTimeAvgMs).c_str());
            Checkbox("VSync", vsync, COLUMN_WIDTH);

            ImGui::NewLine();
        }

        if (ImGui::CollapsingHeader("Viewport", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const int width = canvas->getWidth();
            const int height = canvas->getHeight();

            ImGui::Text(std::format("{}/{} | {} pixels", width, height, formatNumber(width * height)).c_str());

            ImGui::NewLine();
        }

        bool rstAcc = false; //reset frame accumulation flag

        if (ImGui::CollapsingHeader("Raytracer", ImGuiTreeNodeFlags_DefaultOpen))
        {
            RaytracerSettings &raytracerSettings = raytraceComputeLayer->settings;
            DebugSettings &debugSettings = raytraceComputeLayer->debugSettings;

            rstAcc |= Checkbox("Accumulate", raytracerSettings.accumulate, COLUMN_WIDTH);
            rstAcc |= Drag1i("Max Bounces", raytracerSettings.maxBounces, 1, 1, 1e9, COLUMN_WIDTH);
            rstAcc |= Drag1i("Rays Per Pixel", raytracerSettings.raysPerPixel, 1, 1, 128, COLUMN_WIDTH);
            rstAcc |= Checkbox("Environment Light", raytracerSettings.environmentalLight, COLUMN_WIDTH);
            rstAcc |= Drag1f("Diverge Strength", raytracerSettings.divergeStrength, 0.01f, 0, 1e9f, "%.2f", COLUMN_WIDTH);
            rstAcc |= Drag1f("Defocus Strength", raytracerSettings.defocusStrength, 0.01f, 0, 1e9f, "%.2f", COLUMN_WIDTH);

            ImGui::NewLine();

            int currentDrawModeIdx = static_cast<int>(debugSettings.drawMode);
            if (Combo("Draw Debug Mode", &currentDrawModeIdx, DRAW_MODE_NAMES, COLUMN_WIDTH))
            {
                debugSettings.drawMode = static_cast<DrawMode>(currentDrawModeIdx);
                rstAcc = true;
            }

            rstAcc |= Drag1f("Box Threshold", debugSettings.statsThreshold.x, 1, 1, 1e9f, "%.0f", COLUMN_WIDTH);
            rstAcc |= Drag1f("Tri Threshold", debugSettings.statsThreshold.y, 1, 1, 1e9f, "%.0f", COLUMN_WIDTH);

            ImGui::NewLine();
        }

        if (rstAcc)
        {
            AccumulationResetEvent event;
            raytraceComputeLayer->getGlobalMessenger().dispatch(event);
        }

        ImGui::End();
    }

    void SettingsPanel::updateData()
    {
        const engine::ProfilerEntry *renderProfiler = clock->getRenderProfiler();

        lastSpentTimeMs = renderProfiler->getLastSpentTimeMs();
        spentTimeAvgMs = renderProfiler->getSpentTimeAvgMs();
    }
}

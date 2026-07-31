#include "SettingsPanel.h"

#include <imgui.h>
#include "../../../../engine/clock/SimpleClock.h"
#include "../../ImGuiUtils.hpp"
#include "../../../layers/RaytraceComputeLayer.h"
#include "../../../rendering/RenderingCanvas.h"
#include "../../../rendering/RenderingEvents.h"
#include "../../../../engine/messenger/Messenger.hpp"
#include "../../../../engine/window/Window.h"
#include "../viewport/ViewportPanel.h"

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
            static bool vsync = window.isVsync();

            ImGui::Text(std::format("FPS: {} ({:.3f}ms) | Avg: {:.3f}ms", clock->getFPS(), lastSpentTimeMs, spentTimeAvgMs).c_str());
            if(Checkbox("VSync", vsync, COLUMN_WIDTH))
                window.setVsync(vsync);

            ImGui::NewLine();
        }

        if (ImGui::CollapsingHeader("Viewport", ImGuiTreeNodeFlags_DefaultOpen))
        {
            int width = canvas->getWidth();
            int height = canvas->getHeight();
            bool changedResolution = false;

            auto currentDrawStrategy = static_cast<int>(viewportPanel->drawStrategy);
            if (Combo("Draw Strategy", &currentDrawStrategy, VIEWPORT_DRAW_STRATEGIES_NAMES, 128))
            {
                viewportPanel->drawStrategy = static_cast<ViewportDrawStrategy>(currentDrawStrategy);

                if (viewportPanel->drawStrategy == ViewportDrawStrategy::CUSTOM_RESOLUTION)
                {
                    width = viewportPanel->customWidth;
                    height = viewportPanel->customHeight;

                    changedResolution = true;
                }
            }

            if (viewportPanel->drawStrategy == ViewportDrawStrategy::SHRINK_TO_FIT)
            {
                auto currentSTFAspectRatio = static_cast<int>(viewportPanel->shrinkToFitAspectRatio);
                if (Combo("Aspect Ratio", &currentSTFAspectRatio, VIEWPORT_STF_ASPECT_RATIOS_NAMES, 128))
                {
                    viewportPanel->shrinkToFitAspectRatio = static_cast<ViewportSTF_AspectRatios>(currentSTFAspectRatio);
                    viewportPanel->targetAR = ViewportPanel::aspectRatioEnumToFract(viewportPanel->shrinkToFitAspectRatio);
                }
            }
            else
            {
                const int disabledStyles = BeginColumnAlignedControl("Aspect Ratio", 128);

                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::Text(std::format("{:.5f}", viewportPanel->getCustomAspectRatio()).c_str());

                EndColumnAlignedControl(disabledStyles);
            }

            ImGui::NewLine();

            beginDisableIf(viewportPanel->drawStrategy != ViewportDrawStrategy::CUSTOM_RESOLUTION, [&width, &height, &changedResolution] {
                changedResolution |= Drag1i("Width", width, 1, 1, 0, 128);
                changedResolution |= Drag1i("Height", height, 1, 1, 0, 128);
            });

            const int disabledStyles = BeginColumnAlignedControl("Pixel Count", 128);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::Text(formatNumber(width * height).c_str());
            EndColumnAlignedControl(disabledStyles);

            // TODO: this need to be delayed at the beginning of the next frame (or the end of the current one) to prevent flicker
            if (changedResolution)
            {
                canvas->resize(width, height);
                viewportPanel->customWidth = width;
                viewportPanel->customHeight = height;

                AccumulationResetEvent event;
                globalMessenger.dispatch(event);
            }

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
            globalMessenger.dispatch(event);
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

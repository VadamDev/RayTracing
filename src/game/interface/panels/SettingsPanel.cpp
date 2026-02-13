#include "SettingsPanel.h"

#include <format>

#include "../../RaytracingApplication.h"
#include "../../../engine/clock/FixedStepClock.h"
#include "../../controller/CameraController.h"

namespace application
{
    static constexpr float UPDATE_COOLDOWN_SECONDS = 0.5f;

    SettingsPanel::SettingsPanel(const RaytracingApplication *application, Renderer *renderer, CameraController *controller)
        : UIPanel("Settings"), clock(dynamic_cast<engine::FixedStepClock*>(application->getClock())), renderer(renderer), controller(controller) {}

    void SettingsPanel::draw()
    {
        //Prevent data from being updated every render, making the output readable
        if ((updateCooldown += clock->getDeltaTime()) > UPDATE_COOLDOWN_SECONDS)
        {
            updateData();
            updateCooldown = 0;
        }

        ImGui::Begin(name.c_str());

        if (ImGui::CollapsingHeader("Stats", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text(std::format("FPS: {}", clock->getFPS()).c_str());
            ImGui::Text(std::format("{:.3f}ms (avg: {:.3f}ms)", lastRenderTimeMs, renderTimeAvgMs).c_str());

            ImGui::NewLine();
        }

        if (ImGui::CollapsingHeader("Viewport", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const auto framebuffer = renderer->getFramebuffer();
            int viewportWidth = framebuffer->getWidth();
            int viewportHeight = framebuffer->getHeight();

            const auto camera = renderer->getCamera();

            ImGui::Text(std::format("{}x{} | {}px", viewportWidth, viewportHeight, formatNumber(viewportWidth * viewportHeight)).c_str());
            if (ImGui::Combo("##AspectRatio", &selectedAspectRatio, ASPECT_RATIO_LABELS, IM_ARRAYSIZE(ASPECT_RATIO_LABELS)))
                camera->targetAspectRatio = ASPECT_RATIOS[selectedAspectRatio];

            ImGui::NewLine();

            Drag1f("Fov", camera->fov, 1, 75, 130, "%.0f", 128);
            Drag1f("Sensitivity", controller->sensitivity, 0.01f, 0.01f, 1, "%.2f", 128);
            Drag1f("Camera Speed", controller->cameraSpeed, 0.05, 1, 8, "%.1f", 128);
        }

        ImGui::End();
    }

    void SettingsPanel::updateData()
    {
        lastRenderTimeMs = clock->getRenderProfiler()->getLastSpentTimeMs();
        renderTimeAvgMs = clock->getRenderProfiler()->calculateSpentTimeAvgMs();
    }
}

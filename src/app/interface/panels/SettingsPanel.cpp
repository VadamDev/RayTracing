#include "SettingsPanel.h"

#include <format>

#include "../../RaytracingApplication.h"
#include "../../../engine/clock/FixedStepClock.h"
#include "../../controller/CameraController.h"

namespace application
{
    static constexpr float UPDATE_COOLDOWN_SECONDS = 0.25f;
    static constexpr float COLUMN_WIDTH = 150.0f;

    SettingsPanel::SettingsPanel(const RaytracingApplication *application, Renderer *renderer, CameraController *controller)
        : UIPanel("Settings"), clock(dynamic_cast<engine::FixedStepClock*>(application->getClock())), renderer(renderer), controller(controller) {}

    void SettingsPanel::draw()
    {
        // Prevent data from being updated every render, making the output readable
        if ((updateCooldown += clock->getDeltaTime()) > UPDATE_COOLDOWN_SECONDS)
        {
            updateData();
            updateCooldown = 0;
        }

        ImGui::Begin(name.c_str());

        // Stats
        if (ImGui::CollapsingHeader("Stats", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text(std::format("FPS: {} | {:.3f}ms", clock->getFPS(), lastRenderTimeMs).c_str());
            ImGui::Checkbox("Ignore FPS Cap", &clock->bIgnoreFpsCap);

            ImGui::NewLine();
        }

        // Viewport
        if (ImGui::CollapsingHeader("Viewport", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const Canvas *canvas = renderer->getCanvas();
            const int viewportWidth = canvas->getWidth();
            const int viewportHeight = canvas->getHeight();

            const auto camera = renderer->getCamera();

            ImGui::Text(std::format("{}x{} | {}px", viewportWidth, viewportHeight, formatNumber(viewportWidth * viewportHeight)).c_str());
            if (ImGui::Combo("##AspectRatio", &selectedAspectRatio, ASPECT_RATIO_LABELS, IM_ARRAYSIZE(ASPECT_RATIO_LABELS)))
                camera->targetAspectRatio = ASPECT_RATIOS[selectedAspectRatio];

            ImGui::NewLine();
        }

        bool rstAcc = false; //reset frame accumulation flag

        // Camera
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        {
            Camera *camera = renderer->getCamera();

            ImGui::Text(std::format("XYZ: {:.3f}, {:.3f}, {:.3f}", camera->position.x, camera->position.y, camera->position.z).c_str());
            ImGui::Text(std::format("Yaw / Pitch: {:.1f}, {:.1f}", camera->rotation.x, camera->rotation.y).c_str());

            ImGui::NewLine();

            rstAcc |= Drag1f("Fov", camera->fov, 1, 45, 130, "%.0f", COLUMN_WIDTH);
            rstAcc |= Drag1f("Focal Plane", camera->nearClipPlane, 0.01f, 1, 1e9f, "%.2f", COLUMN_WIDTH);

            ImGui::NewLine();

            Drag1f("Sensitivity", controller->sensitivity, 0.01f, 0.01f, 1, "%.2f", COLUMN_WIDTH);
            Drag1f("Camera Speed", controller->cameraSpeed, 0.05f, 1, 1e9f, "%.1f", COLUMN_WIDTH);

            ImGui::NewLine();
        }

        // Raytracer
        if (ImGui::CollapsingHeader("Raytracer", ImGuiTreeNodeFlags_DefaultOpen))
        {
            rstAcc |= Checkbox("Accumulate", renderer->accumulate, COLUMN_WIDTH);
            rstAcc |= Drag1i("Max Bounces", renderer->maxBounces, 1, 1, 1e9, COLUMN_WIDTH);
            rstAcc |= Drag1i("Rays Per Pixel", renderer->raysPerPixel, 1, 1, 128, COLUMN_WIDTH);
            rstAcc |= Checkbox("Environment Light", renderer->environmentLight, COLUMN_WIDTH);
            rstAcc |= Drag1f("Diverge Strength", renderer->divergeStrength, 0.01f, 0, 1e9f, "%.2f", COLUMN_WIDTH);
            rstAcc |= Drag1f("Defocus Strength", renderer->defocusStrength, 0.01f, 0, 1e9f, "%.2f", COLUMN_WIDTH);

            ImGui::NewLine();

            static const std::vector modes = { "Off", "Box Tests", "Tri Tests", "Box + Tri Tests"};
            rstAcc |= Combo("Draw Debug Mode", &renderer->drawDebugMode, modes, COLUMN_WIDTH);
            rstAcc |= Drag1f("Box Threshold", renderer->statsThresholds.x, 1, 1, 1e9f, "%.0f", COLUMN_WIDTH);
            rstAcc |= Drag1f("Tri Threshold", renderer->statsThresholds.y, 1, 1, 1e9f, "%.0f", COLUMN_WIDTH);
        }

        if (rstAcc)
        {
            AccumulationResetEvent event;
            renderer->getGlobalMessenger()->dispatch(event);
        }

        ImGui::End();
    }

    void SettingsPanel::updateData()
    {
        lastRenderTimeMs = clock->getRenderProfiler()->getLastSpentTimeMs();
    }
}

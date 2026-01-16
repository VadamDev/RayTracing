#include "Gui.h"

#include <imgui.h>
#include "../../engine/clock/FixedStepClock.h"

namespace game
{
    void Gui::draw()
    {
        const engine::ProfilerEntry *updateProfiler = clock->getUpdateProfiler();
        const engine::ProfilerEntry *renderProfiler = clock->getRenderProfiler();

        ImGui::Begin("Ray Tracing - Debug");
        ImGui::Text(std::format("UPS: {} ({:.3f}ms) |", clock->getUPS(), updateProfiler->calculateSpentTimeAvgMs()).c_str()); ImGui::SameLine(); ImGui::Checkbox("Unlimited Fps", &bUnlimitedFps);
        ImGui::Text(std::format("FPS: {} ({:.3f}ms)", clock->getFPS(), renderProfiler->calculateSpentTimeAvgMs()).c_str());

        //Camera
        ImGui::NewLine();
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        {
            Camera *camera = renderer->getCamera();

            ImGui::Text(std::format("Position: {:.3f}, {:.3f}, {:.3f}", camera->position.x, camera->position.y, camera->position.z).c_str());
            ImGui::Text(std::format("Yaw: {:.2f} | Pitch: {:.2f}", camera->rotation.y, camera->rotation.x).c_str());

            ImGui::NewLine();

            ImGui::DragFloat("Fov", &camera->fov, 1, 45, 110, "%.0f");
            ImGui::DragFloat("Near Clip Plane", &camera->nearClipPlane, 0.01f, 0.01f, 4, "%.2f");
        }

        //Profilers
        ImGui::NewLine();
        if (ImGui::CollapsingHeader("Profilers"))
        {
            for (auto entry : clock->getProfiler().allEntries())
            {
                ImGui::Text(entry.getName().c_str());
                ImGui::Text(std::format("Avg: {:.3f}ms over {} calls", entry.calculateSpentTimeAvgMs(), entry.getNumCalls()).c_str());
                ImGui::Text(std::format("Spent {:.3f}ms last call", entry.getLastSpentTimeMs()).c_str());

                ImGui::NewLine();
            }
        }

        ImGui::End();

        //Process Choices

        //Unlimited FPS
        if (bUnlimitedFps != clock->bIgnoreFpsCap)
            clock->bIgnoreFpsCap = bUnlimitedFps;
    }
}

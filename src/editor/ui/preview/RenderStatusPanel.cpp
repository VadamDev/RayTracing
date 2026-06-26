#include "RenderStatusPanel.h"

#include <format>
#include <imgui.h>
#include <IconsFontAwesome7.h>

#include "../../../engine/clock/SimpleClock.h"
#include "../../rendering/RenderManager.h"

namespace editor
{
    void RenderStatusPanel::draw(float deltaTime)
    {
        ImGui::Begin(getName(), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking);

        const RenderOptions &options = renderManager->getRenderOptions();
        const RenderData &data = renderManager->getRenderData();

        float completionPercentage = static_cast<float>(data.frames) / options.accumulatedFramesPerImage * 100;
        ImGui::Text(std::format("Rendered {} of {} frames ({:.2f}%%)", data.frames, options.accumulatedFramesPerImage, completionPercentage).c_str());

        const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - data.startTime);
        ImGui::Text(std::format("{:%Hh %Mm %Ss} elapsed", elapsed).c_str());

        const int remainingFrames = options.accumulatedFramesPerImage - data.frames;
        const auto remainingTime = std::chrono::seconds(remainingFrames / clock->getFPS());
        ImGui::Text(std::format("{:%Hh %Mm %Ss} remaining", remainingTime).c_str());

        if (ImGui::Button(ICON_FA_TRASH_CAN " Cancel"))
            renderManager->cancelRender();

        ImGui::End();
    }
}

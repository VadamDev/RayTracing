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
        ImGui::Begin(getName(), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize);

        const RenderOptions &options = renderManager->getRenderOptions();
        const RenderData &data = renderManager->getRenderData();

        // Progress Bar
        const float completion = static_cast<float>(data.frames) / options.accumulatedFramesPerImage;
        const std::string progressText = std::format("{} / {} frames ({:.1f}%)", data.frames, options.accumulatedFramesPerImage, completion * 100);

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.15f, 0.6f, 1, 1));
        ImGui::ProgressBar(completion, ImVec2(-1, 24), "");
        ImGui::PopStyleColor();

        const ImVec2 textSize  = ImGui::CalcTextSize(progressText.c_str());
        const ImVec2 barMin = ImGui::GetItemRectMin();
        const ImVec2 barMax = ImGui::GetItemRectMax();
        ImGui::GetWindowDrawList()->AddText(ImVec2((barMin.x + barMax.x - textSize.x) / 2, (barMin.y + barMax.y - textSize.y) / 2), ImGui::GetColorU32(ImGuiCol_Text), progressText.c_str());

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Time Elapsed/Remaining
        if (ImGui::BeginTable("RenderStats", 2, ImGuiTableFlags_SizingFixedFit))
        {
            ImGui::TableSetupColumn("label", ImGuiTableColumnFlags_WidthFixed, 180);
            ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);

            // Time Elapsed
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextDisabled(ICON_FA_CLOCK " Elapsed:");
            ImGui::TableNextColumn();
            const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - data.startTime);
            ImGui::TextColored(ImVec4(0.85f, 0.85f, 0.85f, 1), std::format("{:%Hh %Mm %Ss}", elapsed).c_str());

            // Time Remaining
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextDisabled(ICON_FA_HOURGLASS_HALF " ETA:");
            ImGui::TableNextColumn();
            const int remainingFrames = options.accumulatedFramesPerImage - data.frames;

            const int currentFps = clock->getFPS();
            if (currentFps > 0 && remainingFrames > 0) {
                const auto remainingTime = std::chrono::seconds(remainingFrames / currentFps);
                ImGui::TextColored(ImVec4(0.5f, 0.9f, 0.15f, 1), std::format("{:%Hh %Mm %Ss}", remainingTime).c_str());
            }else
                ImGui::TextDisabled("--h --m --s");

            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Export Now & Cancel Button
        if (ImGui::Button(ICON_FA_ARROW_UP_FROM_BRACKET " Export Now & Cancel", ImVec2(185, 0)))
            renderManager->cancelRender(true);

        ImGui::SameLine();

        // Cancel Button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.15f, 0.15f, 0.4f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.15f, 0.15f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.2f, 0.2f, 1));

        if (ImGui::Button(ICON_FA_TRASH_CAN " Cancel", ImVec2(-1, 0)))
            renderManager->cancelRender();

        ImGui::PopStyleColor(3);

        ImGui::End();
    }
}

#include "RenderSettingsPanel.h"

#include <IconsFontAwesome7.h>

#include "../../ImGuiUtils.hpp"
#include "../../../rendering/RenderingCanvas.h"
#include <portable-file-dialogs.h>

#include "../../../../engine/messenger/Messenger.hpp"

namespace editor
{
    void RenderSettingsPanel::draw(float deltaTime)
    {
        if (!ImGui::GetIO().WantCaptureKeyboard && ImGui::IsKeyDown(ImGuiKey_Escape))
        {
            CloseMenuEvent event(getName());
            globalMessenger.dispatch(event);

            return;
        }

        const ImGuiViewport *viewport = ImGui::GetMainViewport();

        const auto windowSize = ImVec2(viewport->WorkSize.x / 2, viewport->WorkSize.y / 2);
        const auto windowPos = ImVec2(viewport->WorkPos.x + windowSize.x / 2, viewport->WorkPos.y + windowSize.y / 2);
        ImGui::SetNextWindowSize(windowSize);
        ImGui::SetNextWindowPos(windowPos);

        ImGui::Begin(getName(), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        if (ImGui::BeginTable("renderOptions", 2, ImGuiTableFlags_SizingFixedFit))
        {
            ImGui::TableSetupColumn("label", ImGuiTableColumnFlags_WidthFixed, windowSize.x / 3);
            ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);

            // Export Resolution
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextDisabled(ICON_FA_IMAGE " Use viewport resolution:");
            ImGui::TableNextColumn();
            ImGui::Checkbox("##useViewportResolution", &renderOptions.useViewportResolution);

            beginDisableIf(renderOptions.useViewportResolution, [this] {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextDisabled(ICON_FA_EXPAND " Render Resolution:");
                ImGui::TableNextColumn();

                int newWidth = renderOptions.renderWidth;
                int newHeight = renderOptions.renderHeight;

                bool changed = false;
                if (renderOptions.useViewportResolution)
                {
                    newWidth = canvas->getWidth();
                    newHeight = canvas->getHeight();

                    changed = true;
                }
                else
                {
                    if (newWidth == 0)
                        newWidth = canvas->getWidth();

                    if (newHeight == 0)
                        newHeight = canvas->getHeight();
                }

                const float itemWidth = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) / 2.0f;

                ImGui::BeginGroup();

                ImGui::SetNextItemWidth(itemWidth);
                changed |= ImGui::DragInt("##Width", &newWidth, 1, 1, 3840, "Width: %d px");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(itemWidth);
                changed |= ImGui::DragInt("##Height", &newHeight, 1, 1, 2160, "Height: %d px");

                ImGui::EndGroup();

                if (changed)
                {
                    renderOptions.renderWidth = newWidth;
                    renderOptions.renderHeight = newHeight;
                }
            });

            // Frames per render
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextDisabled(ICON_FA_LAYER_GROUP " Accumulated frames per image:");
            ImGui::TableNextColumn();
            ImGui::DragInt("##accumulatedFramesPerImage", &renderOptions.accumulatedFramesPerImage, 1, 1, std::numeric_limits<int>::max(), "%d frames");

            // (Separator)
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::NewLine();

            // Export Path
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextDisabled(ICON_FA_FOLDER " Filepath:");
            ImGui::TableNextColumn();

            std::string exportPath = renderOptions.exportPath;
            if (exportPath.find_last_of('.') == std::string::npos)
                exportPath += exportFormatToExtension(renderOptions.format);
            if (ImGui::Button(exportPath.c_str(), ImVec2(-1, 0)))
            {
                pfd::save_file dialog("Select a destination", renderOptions.exportPath);
                const std::string result = dialog.result();

                if (!result.empty())
                    renderOptions.exportPath = result;
            }

            // Export Format
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextDisabled(ICON_FA_GEARS " Export Format:");
            ImGui::TableNextColumn();

            auto currentFormatIdx = static_cast<int>(renderOptions.format);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::Combo("##ExportFormat", &currentFormatIdx, EXPORT_FORMAT_NAMES.data(), EXPORT_FORMAT_NAMES.size()))
                renderOptions.format = static_cast<ExportFormat>(currentFormatIdx);

            // JPEG quality
            const bool isJpeg = renderOptions.format == ExportFormat::JPG;
            static int fullQuality = 100;

            beginDisableIf(!isJpeg, [this, isJpeg] {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextDisabled(ICON_FA_SLIDERS " Image Quality:");
                ImGui::TableNextColumn();

                ImGui::SetNextItemWidth(-1);
                ImGui::DragInt("##Quality", isJpeg ? &renderOptions.jpgQuality : &fullQuality, 1.0f, 1, 100, "%d%%");
            });

            ImGui::EndTable();
        }

        // Done (Exit) Button
        ImGui::NewLine();
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 96, ImGui::GetCursorPosY()));
        if (ImGui::Button(ICON_FA_CHECK " Done", ImVec2(96, 0)))
        {
            CloseMenuEvent event(getName());
            globalMessenger.dispatch(event);
        }

        ImGui::End();
    }
}

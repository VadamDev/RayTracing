#include "ViewportPanel.h"

#include <imgui.h>

namespace application
{
    void ViewportPanel::draw()
    {
        ImGui::Begin(name.c_str());

        const float windowWidth = ImGui::GetWindowWidth();
        const float windowHeight = ImGui::GetWindowHeight();

        const auto actualWidth = static_cast<int>(windowHeight * aspectRatio);
        const auto actualHeight = static_cast<int>(windowWidth / aspectRatio);

        if(actualWidth != framebuffer->getWidth() || actualHeight != framebuffer->getHeight())
            framebuffer->resize(actualWidth, actualHeight);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0,0,0,0));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0,0,0,0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));

        ImGui::SetCursorPos(ImVec2((windowWidth - actualWidth) / 2, (windowHeight - actualHeight) / 2));
        const bool clicked = ImGui::ImageButton("viewport", framebuffer->getColorTextureId(), ImVec2(actualWidth, actualHeight), {0, 1}, {1, 0});

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

        ImGui::End();

        if (clicked && !window->isGrabbed())
            window->setGrabbed(true);
    }
}

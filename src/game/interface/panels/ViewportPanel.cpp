#include "ViewportPanel.h"

#include <imgui.h>

namespace application
{
    static constexpr ImVec4 BLANK4(0, 0, 0, 0);
    static constexpr ImVec2 BLANK2(0, 0);

    void ViewportPanel::draw()
    {
        ImGui::Begin(name.c_str());

        const float windowWidth = ImGui::GetWindowWidth();
        const float windowHeight = ImGui::GetWindowHeight();

        const float aspectRatio = renderer->getCamera()->targetAspectRatio;
        const auto actualWidth = static_cast<int>(windowHeight * aspectRatio);
        const auto actualHeight = static_cast<int>(windowWidth / aspectRatio);

        //Terrible! way of detecting resizes
        if(actualWidth != framebuffer->getWidth() || actualHeight != framebuffer->getHeight())
        {
            framebuffer->resize(actualWidth, actualHeight);
            renderer->dispatchViewportResize(engine::WindowResizeEvent(actualWidth, actualHeight));
        }

        ImGui::PushStyleColor(ImGuiCol_Button, BLANK4);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BLANK4);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BLANK4);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, BLANK2);

        ImGui::SetCursorPos(ImVec2((windowWidth - actualWidth) / 2, (windowHeight - actualHeight) / 2));
        if (ImGui::ImageButton("viewport", framebuffer->getColorTextureId(), ImVec2(actualWidth, actualHeight), {0, 1}, {1, 0}) && !window->isGrabbed())
            window->setGrabbed(true);

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

        ImGui::End();
    }
}

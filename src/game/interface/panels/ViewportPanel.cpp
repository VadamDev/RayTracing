#include "ViewportPanel.h"

#include <imgui.h>
#include <spdlog/spdlog.h>

namespace application
{
    static constexpr ImVec4 BLANK4(0, 0, 0, 0);
    static constexpr ImVec2 BLANK2(0, 0);

    void ViewportPanel::draw()
    {
        ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar);

        const float windowWidth = ImGui::GetWindowWidth();
        const float windowHeight = ImGui::GetWindowHeight() - (ImGui::GetFrameHeight() + ImGui::GetStyle().WindowPadding.y);

        const glm::ivec2 accurateDims = calculateViewportSize(windowWidth, windowHeight);

        //Terrible! way of detecting resizes Todo: resize BEFORE rendering (framebuffer swap?)
        if(accurateDims.x != framebuffer->getWidth() || accurateDims.y != framebuffer->getHeight())
            resizeViewport(accurateDims.x, accurateDims.y);

        ImGui::PushStyleColor(ImGuiCol_Button, BLANK4);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BLANK4);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BLANK4);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, BLANK2);

        ImGui::SetCursorPos(ImVec2((windowWidth - accurateDims.x) / 2, (windowHeight - accurateDims.y) / 2));
        if (ImGui::ImageButton("viewport", framebuffer->getColorTextureId(), ImVec2(accurateDims.x, accurateDims.y), {0, 1}, {1, 0}) && !window->isGrabbed())
            window->setGrabbed(true);

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

        ImGui::End();
    }

    void ViewportPanel::resizeViewport(const int newWidth, const int newHeight) const
    {
        framebuffer->resize(newWidth, newHeight);
        renderer->dispatchViewportResize(engine::WindowResizeEvent(newWidth, newHeight));
    }

    glm::ivec2 ViewportPanel::calculateViewportSize(const float windowWidth, const float windowHeight) const
    {
        glm::ivec2 result(0, 0);

        const float viewportAR = windowWidth / windowHeight;
        const float targetAR = renderer->getCamera()->targetAspectRatio;

        if (viewportAR > targetAR)
        {
            result.x = static_cast<int>(windowHeight * targetAR);
            result.y = windowHeight;
        }
        else
        {
            result.x = windowWidth;
            result.y = static_cast<int>(windowWidth / targetAR);
        }

        return result;
    }
}

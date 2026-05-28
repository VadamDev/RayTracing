#include "ViewportPanel.h"

#include "../ImGuiUtils.hpp"
#include "../../../engine/window/Window.h"
#include "../../rendering/RenderingCanvas.h"

namespace editor
{
    void ViewportPanel::draw()
    {
        ImGui::Begin(getName(), nullptr, ImGuiWindowFlags_NoScrollbar);

        const float imguiWidth = ImGui::GetWindowWidth();
        const float imguiHeight = ImGui::GetWindowHeight() - (ImGui::GetFrameHeight() + ImGui::GetStyle().WindowPadding.y); // This weirdness is here to get actual height without the header bar

        // Calculate the size of the rendered image inside the viewport window
        const glm::ivec2 targetDims = calculateViewportSize(imguiWidth, imguiHeight);

        if (targetDims.x != canvas->getWidth() || targetDims.y != canvas->getHeight())
        {
            // TODO: resize canvas to fit imgui window
        }

        const auto drawPos = ImVec2((imguiWidth - targetDims.x) / 2, (imguiHeight - targetDims.y) / 2);

        ImGui::SetCursorPos(drawPos);
        ImGui::Image(canvas->getGLTextureHandle(), ImVec2(targetDims.x, targetDims.y), { 0, 1 }, { 1, 0 });

        ImGui::SetCursorPos(drawPos);
        if (ImGui::InvisibleButton("viewport", ImVec2(targetDims.x ,targetDims.y)))
            window.setGrabbed(true);

        ImGui::End();
    }

    glm::ivec2 ViewportPanel::calculateViewportSize(const float windowWidth, const float windowHeight) const
    {
        glm::ivec2 result(0, 0);

        const float viewportAR = windowWidth / windowHeight;
        const float targetAR = 16.0f / 9.0f;// TODO: renderer->getCamera()->targetAspectRatio;

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

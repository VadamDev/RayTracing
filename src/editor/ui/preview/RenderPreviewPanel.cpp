#include "RenderPreviewPanel.h"

#include <imgui.h>
#include <glm/common.hpp>

#include "../../rendering/RenderingCanvas.h"

namespace editor
{
    void RenderPreviewPanel::draw(float deltaTime)
    {
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::Begin(getName(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus);

        const ImVec2 windowSize = ImGui::GetWindowSize();
        const float canvasWidth = canvas->getWidth();
        const float canvasHeight = canvas->getHeight();

        ImGui::Image(canvas->getGLTextureHandle(), ImVec2(glm::max(windowSize.x, canvasWidth), glm::max(windowSize.y, canvasHeight)), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::PopStyleVar(2);
        ImGui::End();
    }
}

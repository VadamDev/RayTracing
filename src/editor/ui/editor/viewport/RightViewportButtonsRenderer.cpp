#include "RightViewportButtonsRenderer.h"

#include <IconsFontAwesome7.h>

#include "../../../../engine/messenger/Messenger.hpp"
#include "../../../../engine/window/Window.h"
#include "../../../rendering/CameraSystem.h"
#include "../../../rendering/RenderingEvents.h"

namespace editor
{
    static constexpr auto WORLD_RIGHT = glm::vec3(1, 0, 0);
    static constexpr auto WORLD_UP = glm::vec3(0, 1, 0);

    void RightViewportButtonsRenderer::render(const ImVec2 &drawPos) const
    {
        if (!cameraSystem->isCameraPresent())
            return;

        TransformComponent *cameraTransform = cameraSystem->getPrimaryCamera().transform;
        glm::vec3 &cameraPos = cameraTransform->position;
        glm::vec3 &cameraRot = cameraTransform->rotation;

        drawPan(drawPos, cameraPos, cameraRot);
    }
    void RightViewportButtonsRenderer::drawPan(const ImVec2 &drawPos, glm::vec3 &cameraPos, const glm::vec3 &cameraRot) const
    {
        static const float HAND_ICON_WIDTH = ImGui::CalcTextSize(ICON_FA_HAND).x;

        const ImVec2 &framePadding = ImGui::GetStyle().FramePadding;
        const float padX = ImGui::GetContentRegionAvail().x - framePadding.x - HAND_ICON_WIDTH;
        const float padY = framePadding.y;
        ImGui::SetCursorPos(ImVec2(drawPos.x + padX, drawPos.y + padY));

        ImGui::Button(ICON_FA_HAND);
        const bool isActive = ImGui::IsItemActive();
        if (isActive && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            const ImVec2 &mouseDeltas = ImGui::GetIO().MouseDelta;
            if (mouseDeltas.x == 0 && mouseDeltas.y == 0)
                return;

            window.hideCursor(true);

            const auto quat = glm::quat(glm::radians(cameraRot));
            cameraPos += quat * WORLD_RIGHT * (mouseDeltas.x * dragSensitivity);
            cameraPos -= quat * WORLD_UP * (mouseDeltas.y * dragSensitivity);

            PrimaryCameraMovedEvent event;
            globalMessenger.dispatch(event);
        }
        else if (!isActive)
            window.hideCursor(false);
    }
}

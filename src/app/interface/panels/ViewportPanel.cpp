#include "ViewportPanel.h"

#include <ImGuizmo.h>
#include <spdlog/spdlog.h>

#include "HierarchyPanel.h"
#include "../../../engine/messenger/Messenger.hpp"
#include "../../scene/Components.h"

namespace application
{
    static constexpr ImVec4 BLANK4(0, 0, 0, 0);
    static constexpr ImVec2 BLANK2(0, 0);

    static constexpr glm::mat4 FACE_TO_NEG_Z(
        1, 0,  0, 0,
        0, 1,  0, 0,
        0, 0, -1, 0,
        0, 0,  0, 1
    );

    void ViewportPanel::draw()
    {
        ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar);

        /*
         * Viewport
         */

        const float windowWidth = ImGui::GetWindowWidth();
        const float windowHeight = ImGui::GetWindowHeight() - (ImGui::GetFrameHeight() + ImGui::GetStyle().WindowPadding.y);

        const glm::ivec2 accurateDims = calculateViewportSize(windowWidth, windowHeight);

        // Terrible! way of detecting resizes
        if(accurateDims.x != canvas->getWidth() || accurateDims.y != canvas->getHeight())
        {
            CanvasResizeEvent event(accurateDims.x, accurateDims.y);
            renderer->getGlobalMessenger()->dispatch(event);
        }

        ImGui::PushStyleColor(ImGuiCol_Button, BLANK4);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BLANK4);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BLANK4);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, BLANK2);

        const ImVec2 viewportStart = ImVec2((windowWidth - accurateDims.x) / 2, (windowHeight - accurateDims.y) / 2);

        ImGui::SetCursorPos(viewportStart);
        ImGui::Image(canvas->getTextureHandle(), ImVec2(accurateDims.x, accurateDims.y), {0, 1}, {1, 0});

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

        /*
         * Guizmo
        */

        const ImVec2 viewportMin = ImGui::GetItemRectMin();
        const ImVec2 viewportSize = ImGui::GetItemRectSize();

        engine::Entity &selectedEntity = hierarchyPanel->selectedEntity;
        if (selectedEntity)
        {
            if (ImGui::IsKeyPressed(ImGuiKey_T))
                guizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
            else if (ImGui::IsKeyPressed(ImGuiKey_R))
                guizmoOperation = ImGuizmo::OPERATION::ROTATE;
            else if (ImGui::IsKeyPressed(ImGuiKey_S))
                guizmoOperation = ImGuizmo::OPERATION::SCALE;

            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(viewportMin.x, viewportMin.y, viewportSize.x, viewportSize.y);

            Camera *camera = renderer->getCamera();
            const glm::mat4 viewMat = FACE_TO_NEG_Z * glm::inverse(camera->getLocalToWorldMatrix());
            const glm::mat4 &projMat = camera->getProjMatrix();

            auto &transform = selectedEntity.getComponent<TransformComponent>();
            glm::mat4 transformMat = transform.toTransformMat();
            ImGuizmo::Manipulate(glm::value_ptr(viewMat), glm::value_ptr(projMat), guizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(transformMat));

            if (ImGuizmo::IsUsing())
            {
                float matrixTranslation[3], matrixRotation[3], matrixScale[3];
                ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transformMat), matrixTranslation, matrixRotation, matrixScale);

                transform.position = glm::make_vec3(matrixTranslation);
                transform.rotation = glm::make_vec3(matrixRotation);
                transform.scale = glm::make_vec3(matrixScale);

                UpdateShaderBuffersEvent event;
                renderer->getGlobalMessenger()->dispatch(event);
            }
        }
        else
        {
            ImGui::SetCursorPos(viewportStart);
            if (ImGui::InvisibleButton("viewport", ImVec2(accurateDims.x, accurateDims.y)))
                window->setGrabbed(true);
        }

        ImGui::End();
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

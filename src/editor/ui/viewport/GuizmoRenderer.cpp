#include "GuizmoRenderer.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <IconsFontAwesome7.h>

#include "../../../engine/messenger/Messenger.hpp"
#include "../../rendering/CameraSystem.h"
#include "../../rendering/RenderingEvents.h"

namespace editor
{
    static constexpr glm::mat4 FACE_TO_NEG_Z(
        1, 0,  0, 0,
        0, 1,  0, 0,
        0, 0, -1, 0,
        0, 0,  0, 1
    );

    void GuizmoRenderer::processInputs()
    {
        if (ImGui::IsKeyPressed(ImGuiKey_T))
            operation = ImGuizmo::OPERATION::TRANSLATE;
        else if (ImGui::IsKeyPressed(ImGuiKey_R))
            operation = ImGuizmo::OPERATION::ROTATE;
        else if (ImGui::IsKeyPressed(ImGuiKey_S))
            operation = ImGuizmo::OPERATION::SCALE;
    }

    void GuizmoRenderer::render(engine::Entity &selectEntity, const ImVec2 &viewportMin, const ImVec2 &viewportSize) const
    {
        if (!cameraSystem->isCameraPresent() || !selectEntity.hasComponent<TransformComponent>())
            return;

        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(viewportMin.x, viewportMin.y, viewportSize.x, viewportSize.y);

        const glm::mat4 viewMatrix = FACE_TO_NEG_Z * glm::inverse(cameraSystem->getLocalToWorldMatrix());
        const glm::mat4 projMatrix = cameraSystem->getProjectionMatrix();

        auto &transform = selectEntity.getComponent<TransformComponent>();
        glm::mat4 transformMat = transform.toTransformMat();

        ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projMatrix), operation, ImGuizmo::WORLD, glm::value_ptr(transformMat));
        if (ImGuizmo::IsUsing())
        {
            float translation[3], rotation[3], scale[3];
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transformMat), translation, rotation, scale);

            transform.position = glm::make_vec3(translation);
            transform.rotation = glm::make_vec3(rotation);
            transform.scale = glm::make_vec3(scale);

            UpdateRaytracedObjectsBuffersEvent event;
            globalMessenger.dispatch(event);
        }
    }

    void GuizmoRenderer::drawButtons(const ImVec2 &drawPos)
    {
        const ImVec2 &framePadding = ImGui::GetStyle().FramePadding;
        const float padX = framePadding.x * 1.5f;
        const float padY = framePadding.y + ImGui::GetFontSize() + padX * 1.5f;

        ImGui::SetCursorPos(ImVec2(drawPos.x + padX, drawPos.y - padY));

        drawOperationButton(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT, ImGuizmo::OPERATION::TRANSLATE);
        ImGui::SameLine();
        drawOperationButton(ICON_FA_ARROWS_ROTATE, ImGuizmo::OPERATION::ROTATE);
        ImGui::SameLine();
        drawOperationButton(ICON_FA_UP_RIGHT_AND_DOWN_LEFT_FROM_CENTER, ImGuizmo::OPERATION::SCALE);
    }

    void GuizmoRenderer::drawOperationButton(const std::string &title, ImGuizmo::OPERATION operation)
    {
        static constexpr auto activeColor = ImVec4(0, 0.75f, 1, 1);

        const bool isCorrectOperation = operation == this->operation;
        if (isCorrectOperation)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, activeColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
        }

        if (ImGui::Button(title.c_str()))
            this->operation = operation;

        if (isCorrectOperation)
            ImGui::PopStyleColor(3);
    }
}

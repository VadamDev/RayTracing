#pragma once

#include <imgui.h>
#include <ImGuizmo.h>

#include "../../../engine/scene/Entity.h"

namespace engine
{
    class Messenger;
}

namespace editor
{
    class CameraSystem;

    class GuizmoRenderer
    {

    public:
        explicit GuizmoRenderer(engine::Messenger &globalMessenger, CameraSystem *cameraSystem)
            : globalMessenger(globalMessenger), cameraSystem(cameraSystem) {}

        void processInputs();

        void render(engine::Entity &selectEntity, const ImVec2 &viewportMin, const ImVec2 &viewportSize) const;
        void drawButtons(const ImVec2 &drawPos);
    private:
        engine::Messenger &globalMessenger;
        CameraSystem *cameraSystem;

        ImGuizmo::OPERATION operation = ImGuizmo::OPERATION::TRANSLATE;

        void drawOperationButton(const std::string &title, ImGuizmo::OPERATION operation);
    };
}

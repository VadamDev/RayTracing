#pragma once

#include <imgui.h>
#include <glm/vec3.hpp>

namespace engine
{
    class Messenger;
    class Window;
}

namespace editor
{
    class CameraSystem;

    class RightViewportButtonsRenderer
    {

    public:
        explicit RightViewportButtonsRenderer(engine::Window &window, engine::Messenger &globalMessenger, CameraSystem *cameraSystem)
            : window(window), globalMessenger(globalMessenger), cameraSystem(cameraSystem) {}

        void render(const ImVec2 &drawPos) const;

        float dragSensitivity = 0.02f;
    private:
        engine::Window &window;
        engine::Messenger &globalMessenger;
        CameraSystem *cameraSystem;

        void drawPan(const ImVec2 &drawPos, glm::vec3 &cameraPos, const glm::vec3 &cameraRot) const;
    };
}

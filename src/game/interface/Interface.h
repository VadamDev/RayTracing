#pragma once

#include "../../engine/window/imgui/IImGuiWindow.h"
#include "../rendering/Renderer.h"
#include "panels/UIPanel.h"

namespace application
{
    class CameraController;
    class RaytracingApplication;

    class Interface : public engine::IImGuiWindow
    {

    public:
        explicit Interface(engine::Window *window, Renderer *renderer, RaytracingApplication *application, CameraController *controller);

        void draw() override;

    private:
        template<std::derived_from<UIPanel> T, typename... Args>
        std::shared_ptr<T> registerPanel(Args&&... args);

        std::vector<std::shared_ptr<UIPanel>> panels;

        static void setupImGuiStyle();
    };
}

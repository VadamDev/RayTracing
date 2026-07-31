#pragma once

#include <glm/vec2.hpp>

#include "GuizmoRenderer.h"
#include "RightViewportButtonsRenderer.h"
#include "../../UIPanel.h"
#include "../../../rendering/RenderingCanvas.h"

namespace engine
{
    class Window;
}

namespace editor
{
    class RenderingCanvas;
    class HierarchyPanel;

    static const std::vector VIEWPORT_DRAW_STRATEGIES_NAMES = { "Custom", "Shrink to Fit" };
    enum class ViewportDrawStrategy : int
    {
        CUSTOM_RESOLUTION, // Simply use a custom resolution provided by the user
        SHRINK_TO_FIT // Shrink the viewport to fit the program window, maintaining the targetAR
    };

    static const std::vector VIEWPORT_STF_ASPECT_RATIOS_NAMES = { "16:9", "4:3", "9:16" };
    enum class ViewportSTF_AspectRatios : int
    {
        SIXTEEN_NINE, // 16:9
        FOUR_TREE, // 4:3
        NINE_SIXTEEN, // 9:16
    };

    class ViewportPanel : public UIPanel
    {

    public:
        explicit ViewportPanel(engine::Window &window, RenderingCanvas *canvas, HierarchyPanel *hierarchyPanel, engine::Messenger &globalMessenger, CameraSystem *cameraSystem)
            : UIPanel("Viewport"), window(window), globalMessenger(globalMessenger), canvas(canvas), hierarchyPanel(hierarchyPanel), guizmoRenderer(globalMessenger, cameraSystem), rightButtonsRenderer(window, globalMessenger, cameraSystem) {}

        void draw(float deltaTime) override;

        ViewportDrawStrategy drawStrategy = ViewportDrawStrategy::SHRINK_TO_FIT;
        int customWidth = 1280, customHeight = 720;

        ViewportSTF_AspectRatios shrinkToFitAspectRatio = ViewportSTF_AspectRatios::SIXTEEN_NINE;
        float targetAR = aspectRatioEnumToFract(shrinkToFitAspectRatio);

        float getCustomAspectRatio() const
        {
            return static_cast<float>(customWidth) / static_cast<float>(customHeight);
        }

        static constexpr float aspectRatioEnumToFract(const ViewportSTF_AspectRatios aspectRatio)
        {
            switch (aspectRatio)
            {
                using enum ViewportSTF_AspectRatios;

                case SIXTEEN_NINE:
                    return 16.0f / 9.0f;
                case FOUR_TREE:
                    return 4.0f / 3.0f;
                case NINE_SIXTEEN:
                    return 9.0f / 16.0f;
            }

            return 1;
        }
    private:
        engine::Window &window;
        engine::Messenger &globalMessenger;

        RenderingCanvas *canvas;
        HierarchyPanel *hierarchyPanel;

        GuizmoRenderer guizmoRenderer;
        RightViewportButtonsRenderer rightButtonsRenderer;

        glm::ivec2 calculateViewportSize(float windowWidth, float windowHeight) const;
    };
}

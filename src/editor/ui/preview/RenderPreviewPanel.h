#pragma once
#include "../UIPanel.h"

namespace editor
{
    class RenderingCanvas;

    class RenderPreviewPanel : public UIPanel
    {

    public:
        explicit RenderPreviewPanel(RenderingCanvas *canvas)
            : UIPanel("Preview"), canvas(canvas) {}

        void draw(float deltaTime) override;

    private:
        RenderingCanvas *canvas;
    };
}

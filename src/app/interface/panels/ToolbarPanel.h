#pragma once
#include "UIPanel.h"

namespace application {
    class RaytracingApplication;

    class ToolbarPanel : public UIPanel {

    public:
        explicit ToolbarPanel(RaytracingApplication *application)
            : UIPanel("Toolbar"), application(application) {}

        void draw() override;

    private:
        RaytracingApplication *application;
    };
}

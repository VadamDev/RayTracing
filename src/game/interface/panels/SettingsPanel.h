#pragma once

#include <sstream>
#include <spdlog/spdlog.h>

#include "UIPanel.h"
#include "../../rendering/Renderer.h"

namespace engine
{
    class FixedStepClock;
}

namespace application
{
    class RaytracingApplication;
    class CameraController;

    class SettingsPanel : public UIPanel
    {

    public:
        explicit SettingsPanel(const RaytracingApplication *application, Renderer *renderer, CameraController *controller);

        void draw() override;

    private:
        engine::FixedStepClock *clock;
        Renderer *renderer;
        CameraController *controller;

        //Input Data
        const char* ASPECT_RATIO_LABELS[4] { "16/9", "32/9", "4/3", "9/16" };
        const float ASPECT_RATIOS[4] { 16.0f / 9.0f, 32.0f / 9.0f, 4.0f / 3.0f, 9.0f / 16.0f };
        int selectedAspectRatio = 0;

        //Displayed Data
        float updateCooldown = 0;
        float lastRenderTimeMs = 0, renderTimeAvgMs = 0;

        void updateData();

        //Why is there no correct number formating in c++20...
        template<typename T>
        std::string formatNumber(T number, const char separator = ' ')
        {
            const std::string input = std::to_string(number);
            const size_t length = input.length() - 1;

            std::ostringstream oss;
            for (size_t i = 0; i < length + 1; i++)
            {
                oss << input[i];

                if ((length - i) % 3 == 0)
                    oss << separator;
            }

            return oss.str();
        }
    };
}

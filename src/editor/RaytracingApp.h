#pragma once

#include "../engine/Application.h"

namespace editor
{
    class RaytracingApp : public engine::Application
    {

    public:
        using Application::Application;

        void init() override;
        void processInputs(float deltaTime) override;
        void destroy() noexcept override;
    };
}

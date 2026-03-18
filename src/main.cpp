#include <stdexcept>

#include <spdlog/spdlog.h>

#include "engine/window/Window.h"
#include "app/RaytracingApplication.h"
#include "engine/clock/FixedStepClock.h"

using namespace application;

int main()
{
    engine::Window window(1280, 720, "Ray Tracing");

    try { window.create(); }
    catch (std::runtime_error &e)
    {
        spdlog::critical(e.what());
        return -1;
    }

    RaytracingApplication app(window);

    const auto clock = std::make_unique<engine::FixedStepClock>(window, app);
    clock->setTargetFPS(engine::Window::getMonitorRefreshRate());
    clock->bIgnoreFpsCap = true;

    app.start(clock.get());

    return 0;
}

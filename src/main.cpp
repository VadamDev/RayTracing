#include <stdexcept>
#include <spdlog/spdlog.h>

#include "engine/clock/FixedStepClock.h"
#include "engine/window/Window.h"
#include "game/Game.h"

using namespace game;

int main()
{
    engine::Window window(960, 540, "GLFW Window");

    try { window.create(); }
    catch (std::runtime_error &e)
    {
        spdlog::critical(e.what());
        return -1;
    }

    Game game(&window);

    const auto clock = std::make_unique<engine::FixedStepClock>(&window, &game);
    clock->setTargetFPS(engine::Window::getMonitorRefreshRate());

    game.start(clock.get());

    return 0;
}

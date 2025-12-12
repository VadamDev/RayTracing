#include <memory>

#include "engine/clock/FixedStepClock.h"
#include "engine/window/Window.h"
#include "game/Game.h"

using namespace game;

int main()
{
    engine::Window window(960, 540, "GLFW Window");
    if (!window.create())
        return -1;

    Game game(&window);

    const auto clock = std::make_unique<engine::FixedStepClock>(&window, &game);
    game.start(clock.get());

    return 0;
}

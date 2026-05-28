#include "engine/window/Window.h"
#include "editor/RaytracingApp.h"
#include "engine/clock/SimpleClock.h"

using namespace editor;

int main()
{
    engine::Window window(1920, 1080, "Ray Tracing - Editor");
    RaytracingApp app(window);

    engine::SimpleClock clock(window, app);
    app.start(&clock);

    return 0;
}

#include "SimpleClock.h"

#include <exception>
#include <spdlog/spdlog.h>

#include "../IAppLifecycle.h"
#include "../window/Window.h"

namespace engine
{
    /*
     * Managament
     */

    void SimpleClock::start()
    {
        running = true;
        setupProfilers();

        // Init application, window is initialized AFTER because window.create() initialize render layers
        try
        {
            app.init();
            window.create();
        }
        catch (std::exception &e)
        {
            spdlog::critical("A critical exception has been caught during init:\n{}", e.what());
            running = false;
            
            return;
        }

        loop();
    }

    void SimpleClock::stop()
    {
        running = false;
    }

    void SimpleClock::loop()
    {
        using namespace std::chrono;

        int frames = 0;

        auto lastRenderTime = steady_clock::now();
        auto timer = steady_clock::now();

        while (running && !window.shouldClose())
        {
            const auto now = steady_clock::now();
            const int64_t elapsedSinceLastRender = duration_cast<nanoseconds>(now - lastRenderTime).count();

            const float deltaTime = elapsedSinceLastRender / 1e9f;

            try
            {
                renderProfiler->begin();

                app.processInputs(deltaTime);
                window.pushAndPop(deltaTime);

                renderProfiler->end();
            }
            catch (std::runtime_error &e)
            {
                spdlog::critical("An error has been caught during frame render:\n{}", e.what());
                break;
            }

            frames++;
            lastRenderTime = now;

            if (duration_cast<milliseconds>(now - timer).count() >= 1000)
            {
                fps = frames;
                frames = 0;

                timer = now;
            }
        }

        app.destroy();
    }

    void SimpleClock::setupProfilers()
    {
        renderProfiler = profiler.newEntry(RENDER_PROFILER, RENDER_PROFILER_MAX_SAMPLES);
    }
}

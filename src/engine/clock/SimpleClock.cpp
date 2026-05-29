#include "SimpleClock.h"

#include <exception>
#include <spdlog/spdlog.h>

#include "../IAppLifecycle.h"
#include "../window/Window.h"

namespace engine
{
    static constexpr float NANO = 1e9f;

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
            app.onInit();
            window.create();
            app.onPostInit();
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
            const float deltaTime = duration_cast<nanoseconds>(now - lastRenderTime).count() / NANO;

            try
            {
                renderProfiler->begin();

                app.onProcessInputs(deltaTime);
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

        app.onDestroy();
    }

    void SimpleClock::setupProfilers()
    {
        renderProfiler = profiler.newEntry(RENDER_PROFILER, RENDER_PROFILER_MAX_SAMPLES);
    }
}

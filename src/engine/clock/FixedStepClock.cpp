#include "FixedStepClock.h"

#include <chrono>
#include <spdlog/spdlog.h>
#include "../window/Window.h"

using namespace std::chrono;

namespace engine
{
    void FixedStepClock::start()
    {
        bRunning = true;

        try { game.init(); }
        catch (std::exception &e)
        {
            spdlog::critical("A critical exception has been caught during game init:\n{}", e.what());
            return;
        }

        spdlog::info("Starting game clock with target FPS: {} and target UPS: {}", targetFps, targetUps);
        loop();
    }

    void FixedStepClock::stop()
    {
        bRunning = false;
    }

    void FixedStepClock::setupProfilers()
    {
        updateProfiler = gameProfiler.newEntry(UPDATE_PROFILER);
        renderProfiler = gameProfiler.newEntry(RENDER_PROFILER);
    }

    void FixedStepClock::loop()
    {
        setupProfilers();

        int frames = 0, updates = 0;

        auto lastRenderTime = steady_clock::now();
        auto lastUpdateTime = steady_clock::now();

        auto timer = steady_clock::now();

        while (bRunning && !window.shouldClose())
        {
            const auto now = steady_clock::now();

            const int64_t elapsedSinceLastUpdate = duration_cast<nanoseconds>(now - lastUpdateTime).count();
            const int64_t elapsedSinceLastRender = duration_cast<nanoseconds>(now - lastRenderTime).count();

            if (elapsedSinceLastUpdate >= updateTime)
            {
                updateProfiler->begin();

                try { game.update(); }
                catch (std::runtime_error &e)
                {
                    spdlog::critical("An error has been caught during game update:\n{}", e.what());
                    break;
                }

                updates++;
                lastUpdateTime = now;

                updateProfiler->end();
            }
            else if (bIgnoreFpsCap || elapsedSinceLastRender >= renderTime)
            {
                renderProfiler->begin();

                deltaTime = static_cast<float>(elapsedSinceLastRender) / NANO_F;

                try
                {
                    window.pushFrame();

                    game.processInputs(deltaTime);
                    game.render(deltaTime);

                    window.popFrame();
                }
                catch (std::runtime_error &e)
                {
                    spdlog::critical("An error has been caught during frame render:\n{}", e.what());
                    break;
                }

                frames++;
                lastRenderTime = now;

                renderProfiler->end();
            }

            if (duration_cast<milliseconds>(now - timer).count() >= 1000)
            {
                ups = updates;
                fps = frames;

                updates = 0;
                frames = 0;

                timer = now;
            }
        }

        game.destroy();
    }
}

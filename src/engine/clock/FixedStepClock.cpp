#include "FixedStepClock.h"

#include <chrono>
#include <spdlog/spdlog.h>

using namespace std::chrono;

namespace engine
{
    void FixedStepClock::start()
    {
        running = true;

        try { game->init(); }
        catch (std::exception &e)
        {
            spdlog::critical("A critical exception has been caught during game init:\n{}", e.what());
            return;
        }

        loop();
    }

    void FixedStepClock::stop()
    {
        running = false;
    }

    void FixedStepClock::loop()
    {
        int frames = 0, updates = 0;

        auto lastRenderTime = steady_clock::now();
        auto lastUpdateTime = steady_clock::now();

        auto timer = steady_clock::now();

        while (running && !window->shouldClose())
        {
            const auto now = steady_clock::now();

            const int64_t elapsedSinceLastUpdate = duration_cast<nanoseconds>(now - lastUpdateTime).count();
            const int64_t elapsedSinceLastRender = duration_cast<nanoseconds>(now - lastRenderTime).count();

            if (elapsedSinceLastUpdate >= updateTime)
            {
                try { game->update(); }
                catch (std::runtime_error &e)
                {
                    spdlog::critical("An error has been caught during game update:\n{}", e.what());
                    break;
                }

                updates++;
                lastUpdateTime = now;
            }
            else if (ignoreFpsCap || elapsedSinceLastRender >= renderTime)
            {
                const float deltaTime = elapsedSinceLastRender / NANO_F;

                try
                {
                    game->processInputs(deltaTime);

                    window->pushFrame();
                    game->render(deltaTime);
                    window->popFrame();
                }
                catch (std::runtime_error &e)
                {
                    spdlog::critical("An error has been caught during frame render:\n{}", e.what());
                    break;
                }

                frames++;
                lastRenderTime = now;
            }

            if (duration_cast<milliseconds>(now - timer).count() >= 1000)
            {
                ups = updates;
                fps = frames;

                updates = 0;
                frames = 0;

                timer = now;

                spdlog::info("FPS: {} | UPS: {}", fps, ups);
            }
        }

        game->destroy();
    }
}

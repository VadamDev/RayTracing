#include "FixedStepClock.h"
#include <chrono>
#include <iostream>

using namespace std::chrono;

namespace engine
{
    void FixedStepClock::start()
    {
        running = true;
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
                game->update();

                updates++;
                lastUpdateTime = now;
            }
            else if (elapsedSinceLastRender >= renderTime)
            {
                const float deltaTime = elapsedSinceLastRender / NANO_F;

                game->processInputs(deltaTime);

                window->pushFrame();
                game->render(deltaTime);
                window->popFrame();

                frames++;
                lastRenderTime = now;
            }

            if (duration_cast<milliseconds>(now - timer).count() >= 1000)
            {
                std::cout << "FPS: " << frames << " | UPS: " << updates << std::endl;

                updates = 0;
                frames = 0;

                timer = now;
            }
        }
    }
}

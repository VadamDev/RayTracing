#pragma once

#include "GameLifecycle.h"
#include "window/Window.h"
#include "clock/AbstractClock.h"

namespace engine
{
    class AbstractGame : public GameLifecycle
    {

    public:
        explicit AbstractGame(Window *window)
            : window(window) {}

        /*
           Start / Stop
         */
        
        void start(AbstractClock *clock) noexcept
        {
            if (clock != nullptr && clock->isRunning())
                return;

            this->clock = clock;
            this->clock->start();
        }

        void stop() const noexcept
        {
            if (clock == nullptr || !clock->isRunning())
                return;

            clock->stop();
        }

        /*
           Getters
         */

        Window* getWindow() const { return window; }

    private:
        Window *window;
        AbstractClock *clock = nullptr;
    };
}

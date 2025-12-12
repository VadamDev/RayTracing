#pragma once

#include "GameLogic.h"
#include "clock/AbstractClock.h"
#include "window/Window.h"

namespace engine
{
    class AbstractGame : public GameLogic
    {

    public:
        explicit AbstractGame(Window *window)
            : window(window) {}

        void start(AbstractClock *clock)
        {
            this->clock = clock;
            clock->start();
        }

        void stop() const
        {
            clock->stop();
        }

        Window* getWindow() const { return window; }

    private:
        Window *window;
        AbstractClock *clock;
    };
}

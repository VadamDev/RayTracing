#pragma once

#include "../window/Window.h"
#include "../GameLifecycle.h"

namespace engine
{
    class AbstractClock
    {

    public:
        explicit AbstractClock(Window *window, GameLifecycle *game)
            : window(window), game(game) {}

        virtual ~AbstractClock() = default;

        /*
           Start / Stop
         */

        virtual void start() = 0;
        virtual void stop() = 0;

        /*
           Getters
         */

        virtual int getFPS() const = 0;
        bool isRunning() const { return running; }

    protected:
        Window *window;
        GameLifecycle *game;

        bool running = false;
    };
}

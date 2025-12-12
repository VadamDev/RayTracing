#pragma once

#include "../window/Window.h"
#include "../GameLogic.h"

namespace engine
{
    class Window;

    class AbstractClock
    {

    public:
        explicit AbstractClock(Window *window, GameLogic *game)
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

        virtual int getFPS() = 0;
        bool isRunning() const { return running; }

    protected:
        Window *window;
        GameLogic *game;

        bool running = false;
    };
}

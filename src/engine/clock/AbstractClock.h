#pragma once

#include "../window/Window.h"
#include "../IGameLifecycle.h"

namespace engine
{
    class AbstractClock
    {

    public:
        explicit AbstractClock(Window &window, IGameLifecycle &game)
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
        bool isRunning() const { return bRunning; }

        float getDeltaTime() const { return deltaTime; }

    protected:
        Window &window;
        IGameLifecycle &game;

        bool bRunning = false;

        float deltaTime;
    };
}

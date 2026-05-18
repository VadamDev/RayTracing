#pragma once

#include "IAppLifecycle.h"
#include "window/Window.h"
#include "clock/AbstractClock.h"
#include "messenger/Messenger.hpp"

namespace engine
{
    class Application : public IAppLifecycle
    {

    public:
        explicit Application(Window &window)
            : window(window)
        {
            window.messenger = &globalMessenger;
        }

        void start(AbstractClock *clock) noexcept
        {
            if (this->clock != nullptr && this->clock->isRunning())
                return;

            this->clock = clock;
            clock->start();
        }

        void stop() const noexcept
        {
            if (clock == nullptr || !clock->isRunning())
                return;

            clock->stop();
        }

        /*
         * Getters
         */

        Window& getWindow() const { return window; }
        AbstractClock* getClock() const { return clock; }
        Messenger& getGlobalMessenger() { return globalMessenger; }

    protected:
        Window &window;
        AbstractClock *clock = nullptr;

        Messenger globalMessenger;
    };
}

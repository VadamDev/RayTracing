#pragma once

namespace engine
{
    class Window;
    class IAppLifecycle;

    class AbstractClock
    {

    public:
        explicit AbstractClock(Window &window, IAppLifecycle &app)
            : window(window), app(app) {}
        virtual ~AbstractClock() = default;

        /*
         * Management
         */

        virtual void start() = 0;
        virtual void stop() = 0;

        /*
         * Getters
         */

        virtual int getFPS() const = 0;
        bool isRunning() const { return running; }

    protected:
        Window &window;
        IAppLifecycle &app;

        bool running = false;
    };
}

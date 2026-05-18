#pragma once

#include "AbstractClock.h"
#include "../profiler/SteadyProfiler.h"

namespace engine
{
    static constexpr auto RENDER_PROFILER = "Render";
    static constexpr int RENDER_PROFILER_MAX_SAMPLES = 1000;

    class SimpleClock : public AbstractClock
    {

    public:
        using AbstractClock::AbstractClock;

        /*
         * Management
         */

        void start() override;
        void stop() override;

        /*
         * Getters
         */

        int getFPS() const override { return fps; }

        SteadyProfiler& getProfiler() { return profiler; }
        ProfilerEntry* getRenderProfiler() const { return renderProfiler; }

    private:
        int fps = 0;

        SteadyProfiler profiler;
        ProfilerEntry *renderProfiler = nullptr;

        void setupProfilers();
        void loop();
    };
}

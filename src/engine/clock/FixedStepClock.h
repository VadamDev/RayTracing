#pragma once

#include "AbstractClock.h"
#include "../profiler/SteadyProfiler.h"

namespace engine
{
    static constexpr int64_t NANO = 1e9;
    static constexpr float NANO_F = 1e9f;

    class FixedStepClock : public AbstractClock {

    public:
        using AbstractClock::AbstractClock;

        /*
           Start / Stop
         */

        void start() override;
        void stop() override;

        /*
           Getters
         */

        int getUPS() const { return ups; }
        int getFPS() const override { return fps; }

        /*
           Setters
         */

        void setTargetUPS(const int targetUps)
        {
            this->targetUps = targetUps;
            updateTime = NANO / targetUps;
        }

        void setTargetFPS(const int targetFps)
        {
            this->targetFps = targetFps;
            renderTime = NANO / targetFps;
        }

        bool bIgnoreFpsCap = false;

    private:
        int targetUps = 30, targetFps = 60;
        int64_t updateTime = NANO / targetUps, renderTime = NANO / targetFps;

        int ups = 0, fps = 0;

        SteadyProfiler gameProfiler;

        ProfilerEntry *updateProfiler = nullptr;
        ProfilerEntry *renderProfiler = nullptr;

        void setupProfilers();
        void loop();
    };
}

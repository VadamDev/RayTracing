#pragma once

#include "AbstractClock.h"

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
        int getFPS() override { return fps; }



        /*
           Setters
         */

        void setTargetUPS(int targetUps)
        {
            this->targetUps = targetUps;
            updateTime = NANO / targetUps;
        }

        void setTargetFPS(int targetFps)
        {
            this->targetFps = targetFps;
            renderTime = NANO / targetFps;
        }

    private:
        int targetUps = 30, targetFps = 60;
        int64_t updateTime = NANO / targetUps, renderTime = NANO / targetFps;

        int ups = 0, fps = 0;

        void loop();
    };
}

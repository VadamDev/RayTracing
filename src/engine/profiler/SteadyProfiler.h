#pragma once

#include <chrono>
#include <functional>
#include <unordered_map>
#include <spdlog/spdlog.h>

namespace engine {
    using namespace std::chrono;

    class ProfilerEntry;

    /*
     * SteadyProfiler
     */

    class SteadyProfiler
    {

    public:
        ProfilerEntry* newEntry(const std::string &name);
        void forEachEntries(const std::function<void(ProfilerEntry&)> &func);

        nanoseconds calculateTotalSpentTime();
        uint64_t calculateTotalNumCalls();

        uint64_t profilersCount() const { return entries.size(); }

    private:
        std::unordered_map<std::string, ProfilerEntry> entries;
    };

    /*
     * ProfilerEntry
     */

    class ProfilerEntry
    {

    public:
        explicit ProfilerEntry(std::string name)
            : name(std::move(name))
        {
            startTime = steady_clock::now();
        }

        /*
         * Management
         */

        void begin();
        void end();

        void reset();
        void stop();

        /*
         * Getters
         */

        std::string& getName() { return name; }

        time_point<steady_clock> getStartTime() const { return startTime; }
        time_point<steady_clock> getEndTime() const { return endTime; }

        nanoseconds getAccumulatedTime() const { return accumulatedTime; }
        nanoseconds getLastSpentTime() const { return lastSpentTime; }

        uint64_t getNumCalls() const { return numCalls; }

        nanoseconds calculateSpentTimeAvg() const;

        bool isProfiling() const { return bProfiling; }
        bool isClosed() const { return bClosed; }

    private:
        std::string name;

        time_point<steady_clock> startTime, endTime;
        time_point<steady_clock> beginTime;

        nanoseconds accumulatedTime = nanoseconds::zero(), lastSpentTime = nanoseconds::zero();
        uint64_t numCalls = 0;

        bool bProfiling = false, bClosed = false;
    };
}

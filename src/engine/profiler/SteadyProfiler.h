#pragma once

#include <chrono>
#include <unordered_map>

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
        ProfilerEntry& retrieveEntry(const std::string &name);

        std::vector<ProfilerEntry> allEntries();

        nanoseconds calculateTotalSpentTime();
        float calculateTotalSpentTimeMs() { return calculateTotalSpentTime().count() / 1e6f; }

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

        //Start / End time of the profiler
        time_point<steady_clock> getStartTime() const { return startTime; }
        time_point<steady_clock> getEndTime() const { return endTime; }

        //Total accumulated time since profiler started
        nanoseconds getAccumulatedTime() const { return accumulatedTime; }

        //Last spent time
        nanoseconds getLastSpentTime() const { return lastSpentTime; }
        float getLastSpentTimeMs() const { return getLastSpentTime().count() / 1e6f; }

        //Amount of time the profiler has run
        uint64_t getNumCalls() const { return numCalls; }

        //Average time spent
        nanoseconds calculateSpentTimeAvg() const;
        float calculateSpentTimeAvgMs() const { return calculateSpentTimeAvg().count() / 1e6f; }

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

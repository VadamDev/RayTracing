#include "SteadyProfiler.h"

namespace engine {
    /*
     * SteadyProfiler
     */

    ProfilerEntry* SteadyProfiler::newEntry(const std::string &name)
    {
        auto [it, _] = entries.try_emplace(name, name);
        return &it->second;
    }

    void SteadyProfiler::forEachEntries(const std::function<void(ProfilerEntry&)> &func)
    {
        for (auto& [_, entry] : entries)
            func(entry);
    }

    nanoseconds SteadyProfiler::calculateTotalSpentTime()
    {
        nanoseconds total = nanoseconds::zero();

        for (auto& [_, entry] : entries)
            total += entry.getAccumulatedTime();

        return total;
    }

    uint64_t SteadyProfiler::calculateTotalNumCalls()
    {
        uint64_t total = 0;

        for (auto& [_, entry] : entries)
            total += entry.getNumCalls();

        return total;
    }

    /*
     * ProfilerEntry
     */

    void ProfilerEntry::begin()
    {
        if (bClosed)
            throw std::runtime_error("Failed to begin profiler, it has been closed!");

        if (bProfiling)
            throw std::runtime_error("A profiler has already been started");

        beginTime = steady_clock::now();
        bProfiling = true;
    }

    void ProfilerEntry::end()
    {
        if (bClosed)
            throw std::runtime_error("Failed to end profiler, it has been closed!");

        if (!bProfiling)
            throw std::runtime_error("The profiler hasn't been started");

        const nanoseconds durationNs = steady_clock::now() - beginTime;

        accumulatedTime += durationNs;
        lastSpentTime = durationNs;

        numCalls++;

        bProfiling = false;
    }

    void ProfilerEntry::reset()
    {
        accumulatedTime = nanoseconds::zero();
        lastSpentTime = nanoseconds::zero();

        numCalls = 0;
    }

    void ProfilerEntry::stop()
    {
        if (bClosed)
            throw std::runtime_error("Failed to begin profiler, it has been closed!");

        if(bProfiling)
            end();

        bClosed = true;
        endTime = steady_clock::now();
    }

    nanoseconds ProfilerEntry::calculateSpentTimeAvg() const
    {
        if (numCalls <= 0)
            return nanoseconds::zero();

        return accumulatedTime / numCalls;
    }

}

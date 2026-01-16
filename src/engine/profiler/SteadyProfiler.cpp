#include "SteadyProfiler.h"

#include <ranges>

namespace engine {
    /*
     * SteadyProfiler
     */

    ProfilerEntry* SteadyProfiler::newEntry(const std::string &name)
    {
        if (entries.contains(name))
            throw std::runtime_error("A profiler with that name already exists!");

        auto [it, _] = entries.try_emplace(name, name);
        return &it->second;
    }

    ProfilerEntry& SteadyProfiler::retrieveEntry(const std::string &name)
    {
        return entries.at(name);
    }

    std::vector<ProfilerEntry> SteadyProfiler::allEntries()
    {
        std::vector<ProfilerEntry> result;
        result.reserve(entries.size());

        for(const auto &entry : entries | std::views::values)
            result.push_back(entry);

        return result;
    }

    nanoseconds SteadyProfiler::calculateTotalSpentTime()
    {
        nanoseconds total = nanoseconds::zero();

        for (auto &entry: entries | std::views::values)
            total += entry.getAccumulatedTime();

        return total;
    }

    uint64_t SteadyProfiler::calculateTotalNumCalls()
    {
        uint64_t total = 0;

        for (auto &entry: entries | std::views::values)
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

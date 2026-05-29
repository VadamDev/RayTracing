#include "SteadyProfiler.h"

#include <numeric>
#include <ranges>

namespace engine
{
    /*
     * Profiler
     */

    using namespace std::chrono;

    ProfilerEntry* SteadyProfiler::newEntry(const std::string &name, int maxSamples)
    {
        if (entries.contains(name))
            throw exceptions::ProfilerException("An entry with that name already exists!");

        auto [it, _] = entries.try_emplace(name, name, maxSamples);
        return &it->second;
    }

    ProfilerEntry& SteadyProfiler::getEntry(const std::string &name)
    {
        return entries.at(name);
    }

    std::vector<ProfilerEntry> SteadyProfiler::allEntries()
    {
        auto values = entries | std::views::values;
        return std::vector(values.begin(), values.end());
    }

    /*
     *  Entry
     */

    void ProfilerEntry::begin()
    {
        if (profiling)
            throw exceptions::ProfilerException("A profiler has already been started");

        beginTime = steady_clock::now();
        profiling = true;
    }

    void ProfilerEntry::end()
    {
        if (!profiling)
            throw exceptions::ProfilerException("The profiler hasn't been started");

        const nanoseconds durationNs = steady_clock::now() - beginTime;
        lastSpentTime = durationNs;

        if (samples.size() >= maxSamples)
            samples.erase(samples.begin());
        samples.push_back(durationNs.count() / 1e6f);

        spentTimeAvgMs = std::accumulate(samples.begin(), samples.end(), 0.0f) / samples.size();

        profiling = false;
    }

    void ProfilerEntry::reset()
    {
        if (profiling)
            end();

        lastSpentTime = nanoseconds::zero();
        samples.clear();
    }
}

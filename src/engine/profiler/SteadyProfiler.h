#pragma once

#include <chrono>
#include <string>
#include <unordered_map>

namespace engine
{
    class ProfilerEntry;

    /*
     * Profiler
     */

    class SteadyProfiler
    {

    public:
        ProfilerEntry* newEntry(const std::string &name, int maxSamples);
        ProfilerEntry& getEntry(const std::string &name);

        std::vector<ProfilerEntry> allEntries();

    private:
        std::unordered_map<std::string, ProfilerEntry> entries;
    };

    /*
     * Entry
     */

    class ProfilerEntry
    {

    public:
        explicit ProfilerEntry(std::string name, const int maxSamples)
            : name(std::move(name)), maxSamples(maxSamples)
        {
            samples.reserve(maxSamples);
        }

        /*
         * Management
         */

        void begin();
        void end();

        void reset();

        /*
         * Getters
         */

        std::string& getName() { return name; }
        int getMaxSamples() const { return maxSamples; }
        bool isProfiling() const { return profiling; }

        std::chrono::time_point<std::chrono::steady_clock> getStartTime() const { return startTime; }
        std::vector<float>& getSamples() { return samples; }

        // Last spent time
        std::chrono::nanoseconds getLastSpentTime() const { return lastSpentTime; }
        float getLastSpentTimeMs() const { return lastSpentTime.count() / 1e6f; }
        float getSpentTimeAvgMs() const { return spentTimeAvgMs; }

    private:
        std::string name;
        int maxSamples;
        bool profiling = false;

        std::chrono::time_point<std::chrono::steady_clock> startTime, beginTime;
        std::vector<float> samples;
        float spentTimeAvgMs = -1;

        std::chrono::nanoseconds lastSpentTime = std::chrono::nanoseconds::zero();
    };

    /*
     * Exceptions
     */

    namespace exceptions
    {
        class ProfilerException : public std::runtime_error
        {

        public:
            explicit ProfilerException(const std::string &message)
                : ProfilerException(message.c_str()) {}

            explicit ProfilerException(const char *message)
                : runtime_error(message) {}
        };
    }
}

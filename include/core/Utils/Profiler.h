#pragma once
#include "common.h"

#include "Timer.h"

#include <float.h>

#define PROFILE_CONCAT_IMPL(x, y) x##y
#define PROFILE_CONCAT(x, y) PROFILE_CONCAT_IMPL(x, y)

#if defined(MC_ENABLE_PROFILER)
#define PROFILE_SCOPE(name) MC::Utils::ProfilerTimer PROFILE_CONCAT(profilerTimer_, __COUNTER__)(name)
#define PROFILE_FUNCTION()  PROFILE_SCOPE(__FUNCTION__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif

namespace MC 
{
    namespace Utils
    {
        struct ProfilerEntry
        {
            const char *name;
            u64t calls = 0;
            double min = DBL_MAX;
            double max = 0.0;
            double avr = 0.0;
            double total = 0.0f;
        };

        class MC_API ProfilerTimer : public ScopedTimer
        {
        public:
            ProfilerTimer(const char *name);
            ~ProfilerTimer() override;
        };

        class MC_API ProfilerManager
        {
        private:
            ProfilerManager();
            ~ProfilerManager();
        public:
            static ProfilerManager &Get();
            void Add(const char *name, double duration);
            void OnTick();
        };
    }   
}
#include "mc.h"

#include "Utils/Profiler.h"

#include "Log/Log.h"

namespace MC 
{
    namespace Utils
    {
        static std::unordered_map<const char*, ProfilerEntry> s_Entries;

        ProfilerTimer::ProfilerTimer(const char *name)
            : ScopedTimer(name)
        {

        }

        ProfilerTimer::~ProfilerTimer()
        {
            ProfilerManager::Get().Add(m_Name, m_Timer.ElapsedMillis());
        }

        ProfilerManager::ProfilerManager()
        {

        }

        ProfilerManager::~ProfilerManager()
        {

        }

        ProfilerManager &ProfilerManager::Get()
        {
            static ProfilerManager instance;
            return instance;
        }

        void ProfilerManager::Add(const char *name, double duration)
        {
            auto &result = s_Entries[name];
            result.name = name;
            result.calls++;
            result.min = std::min(result.min, duration);
            result.max = std::max(result.max, duration);
            result.avr += (duration - result.avr) / result.calls;
            result.total += duration;
        }

        void ProfilerManager::OnTick()
        {
            std::ostringstream oss;
            oss << "\n" << std::left
                << std::setw(35) << "Function"
                << std::setw(12) << "Calls"
                << std::setw(15) << "AVR (ms)"
                << std::setw(12) << "Min (ms)"
                << std::setw(12) << "Max (ms)" 
                << std::setw(12) << "Total (ms)" << "\n";
            oss << "--------------------------------------------------------\n";
            
            for (const auto &[name, ent] : s_Entries) {
                oss << std::left
                    << std::setw(35) << ent.name
                    << std::setw(12) << ent.calls
                    << std::setw(15) << std::fixed << std::setprecision(4) << ent.avr
                    << std::setw(12) << ent.min
                    << std::setw(12) << ent.max  
                    << std::setw(12) << ent.total << "\n";
            }

            oss << "========================================================\n";

            mc_trace("{}", oss.str());

            s_Entries.clear();
        }
    }
}

#include "mc.h"

#include "Utils/Timer.h"

#include "Log/Log.h"

#include <chrono>

namespace MC 
{
	namespace Utils 
	{
		struct Timer::PImpl
		{
			std::chrono::high_resolution_clock::time_point Start;
		};

		Timer::Timer()
		{
			m_Impl = new PImpl();
			this->Reset();
		}

		Timer::~Timer()
		{
			if (m_Impl)
				delete m_Impl;
		}

		void Timer::Reset() 
		{
			m_Impl->Start = std::chrono::high_resolution_clock::now();
		}

		float Timer::ElapsedMillis() 
		{
			auto now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration<float, std::milli>(now - m_Impl->Start).count();
		}

		float Timer::ElapsedSeconds() 
		{
			return this->ElapsedMillis() / 1000.0f;
		}

		ScopedTimer::ScopedTimer(const char *name)
			: m_Name(name)
		{
			m_Timer.Reset();
		}

		ScopedTimer::~ScopedTimer()
		{
			
		}

	}
}
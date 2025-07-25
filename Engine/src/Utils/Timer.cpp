
#include "Timer.h"

namespace MC {
	namespace Utils {

		Timer::Timer() 
		{
			this->Reset();
		}

		void Timer::Reset() 
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		float Timer::ElapsedMillis() 
		{
			auto now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration<float, std::milli>(now - this->m_Start).count();
		}

		float Timer::ElapsedSeconds() 
		{
			return this->ElapsedMillis() / 1000.0f;
		}

	}
}
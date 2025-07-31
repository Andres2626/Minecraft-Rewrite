#pragma once
#include "common.h"

#include <chrono>

namespace MC {
	namespace Utils {

		class Timer {
		protected:
			std::chrono::high_resolution_clock::time_point m_Start;
		public:
			Timer();

			/* reset timer */
			void Reset();

			/* return ellapsed milliseconds timer */
			float ElapsedMillis();

			/* return ellapsed seconds of timer */
			float ElapsedSeconds();
		};

	}
};
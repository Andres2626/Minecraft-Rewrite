#pragma once
#include "common.h"

namespace MC {
	namespace Utils {

		class Timestep {
		private:
			float m_Delta;
			float m_LastTime;
		public:
			inline Timestep(float initial) 
				: m_Delta(0.0f), m_LastTime(initial)
			{  }

			/* update delta time */
			inline void Update(float current) 
			{
				m_Delta = current - this->m_LastTime;
				m_LastTime = m_Delta;
			}

			inline float GetMillis() const { return this->m_Delta; };
			inline float GetSeconds() const { return this->m_Delta * 0.001f; }
		};

	}
}
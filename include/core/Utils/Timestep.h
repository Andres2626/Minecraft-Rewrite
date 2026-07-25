#pragma once
#include "common.h"

namespace MC 
{
	namespace Utils 
	{

		class Timestep 
		{
		private:
			float m_Delta;
			float m_LastTime;
		public:
			Timestep()
				: m_Delta(0.0f), m_LastTime(0.0f)
			{ }

			Timestep(float initial) 
				: m_Delta(0.0f), m_LastTime(initial)
			{  }

			/* update delta time */
			void Update(float current) 
			{
				m_Delta = current - this->m_LastTime;
				m_LastTime = current;
			}

			inline float GetMillis() const { return this->m_Delta; };
			inline float GetSeconds() const { return this->m_Delta * 0.001f; }
		};

	}
}
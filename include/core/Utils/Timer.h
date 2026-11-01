#pragma once
#include "common.h"

#include "NonCopyable.h"
#include "NonMovable.h"

namespace MC 
{
	struct PImpl;

	namespace Utils 
	{

		class MC_API Timer : public NonCopyable, public NonMovable
		{
		protected:
			struct PImpl;
			PImpl *m_Impl;
		public:
			Timer();
			~Timer();
			/* reset timer */
			void Reset();

			/* return ellapsed milliseconds timer */
			float ElapsedMillis();

			/* return ellapsed seconds of timer */
			float ElapsedSeconds();
		};


		class MC_API ScopedTimer : public NonCopyable, public NonMovable
		{
		protected:
			Timer m_Timer;
			const char *m_Name;
		public:
			ScopedTimer(const char *name);
			virtual ~ScopedTimer();
		};
	}
};
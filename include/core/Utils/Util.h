#pragma once
#include "common.h"

#include "Profiler.h"

#if defined(_DEBUG)
#define mc_assert(cond, msg) \
	if (!(cond)) { \
		MC::Internal::debug_assert(#cond, __func__, msg, __FILE__, __LINE__); \
	}
#else
#define mc_assert(...) ((void)0)
#endif

namespace MC 
{
	namespace Internal 
	{
		void MC_API debug_assert(const char *cond, const char *func, const char *msg, const char *file, int line);
	}
}
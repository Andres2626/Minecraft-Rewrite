#pragma once
#include "common.h"

#ifndef MC_USE_RELEASE
#define mc_assert(x, ...) \
	if (!(x)) { \
		MC::Internal::mc_printf("An assertion has ocurred in %s:%i\n", __FILE__, __LINE__); \
		MC::Internal::mc_printf("%s\n", #x); \
		MC::Internal::mc_printf(__VA_ARGS__); \
 		MC::Internal::mc_breakpoint(); \
	}
#else
#define mc_assert(...)
#endif

namespace MC 
{
	namespace Internal 
	{

		int MC_API mc_printf(const char* fmt, ...);
		void MC_API mc_breakpoint();

	}
}
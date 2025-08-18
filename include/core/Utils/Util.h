#pragma once
#include "common.h"

#ifndef MC_USE_RELEASE
#define mc_assert(x, ...) \
	if (!(x)) { \
		MC::Internal::printf("An assertion has ocurred in %s:%i\n", __FILE__, __LINE__); \
		MC::Internal::printf("%s\n", #x); \
		MC::Internal::printf(__VA_ARGS__); \
		MC::Internal::printf("\n"); \
 		MC::Internal::breakpoint(); \
	}
#else
#define mc_assert(...)
#endif

namespace MC 
{
	namespace Internal 
	{

		int MC_API printf(const char* fmt, ...);
		void MC_API breakpoint();

	}
}
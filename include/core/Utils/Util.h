#pragma once
#include "common.h"

#ifndef MC_USE_RELEASE
#define mc_assert(x, ...) \
	if (!(x)) { \
		printf("An assertion has ocurred in %s:%i\n", __FILE__, __LINE__); \
		printf("%s\n", #x); \
		printf(__VA_ARGS__); \
 		MC_INTERNAL_BREAK_POINT; \
	}
#else
#define mc_assert(...)
#endif

namespace MC 
{
	namespace Internal 
	{

	}
}
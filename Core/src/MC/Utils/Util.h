#pragma once
#include "common.h"

#ifndef NDEBUG
#define MC_ASSERT(x, fmt, ...) \
	if (!(x)) { \
		MC::internal_printf("An assertion has ocurred in %s:%i\n", __FILE__, __LINE__); \
		MC::internal_printf("%s\n", #x); \
		MC::internal_printf(fmt, __VA_ARGS__); \
 		MC::internal_breakpoint(); \
	}
#else
#define MC_ASSERT(...)
#endif

namespace MC 
{
	int MC_API internal_printf(const char* fmt, ...);
	void MC_API internal_breakpoint();
}
#pragma once
#include "common.h"

#define RD_ABORT (x, fmt, ...) \
	if (!(x)) { \
		MC::rd_internal_printf("Abort called in %s:%i\n", __FILE__, __LINE__); \
		MC::internal_printf("%s\n", #x); \
		MC::internal_printf(fmt, __VA_ARGS__); \
 		abort(); \
	}

#ifndef NDEBUG
#define RD_ASSERT(x, fmt, ...) \
	if (!(x)) { \
		MC::internal_printf("An assertion has ocurred in %s:%i\n", __FILE__, __LINE__); \
		MC::internal_printf("%s\n", #x); \
		MC::internal_printf(fmt, __VA_ARGS__); \
 		MC::internal_breakpoint(); \
	}
#else
#define RD_ASSERT(...)
#endif

namespace MC {
	int RD_API internal_printf(const char* fmt, ...);
	void RD_API internal_breakpoint();
}
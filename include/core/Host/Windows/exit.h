#pragma once
#include "common.h"

#ifdef MC_USE_CYGWIN
#include <signal.h>
#define MC_INTERNAL_BREAK_POINT raise(SIGTRAP)
#else
#define MC_INTERNAL_BREAK_POINT __debugbreak()
#endif
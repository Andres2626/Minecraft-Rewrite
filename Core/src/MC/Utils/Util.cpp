#include "util.h"

#include "Host/exit.h"

#include <stdarg.h>

namespace MC 
{

	int internal_printf(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);

		int ret = vprintf(fmt, args);

		va_end(args);

		return ret;
	}

	void internal_breakpoint()
	{
		MC_INTERNAL_BREAK_POINT;
	}

}



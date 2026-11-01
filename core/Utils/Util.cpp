#include "mc.h"

#include "Utils/Util.h"

#include "Log/Log.h"

namespace MC 
{
	namespace Internal
	{
		void debug_assert(const char *cond, const char *func, const char* msg, const char *file, int line)
		{
			mc_fatal("An assertion has ocurred in {}:{}\n", file, line);
			mc_fatal("Condition: {}", cond);
			mc_fatal("Function: {}", func);
			mc_fatal("{}", msg);
			MC_INTERNAL_BREAK_POINT;
		}
	}
}



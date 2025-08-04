#pragma once
#include "common.h"

namespace MC 
{
	namespace Graphics 
	{
		/* Values stoled from glad/glad.h */
		enum class BlendValue 
		{
			ZERO = 0,
			ONE = 1,
			SRC_COLOR = 0x300,
			ONE_MINUS_SRC_COLOR,
			SRC_ALPHA,
			ONE_MINUS_SRC_ALPHA,
			DST_COLOR = 0x306,
			ONE_MINUS_DST_COLOR,
			CONSTANT_COLOR = 0x8001,
			ONE_MINUS_CONSTANT_COLOR,
			CONSTANT_ALPHA,
			ONE_MINUS_CONSTANT_ALPHA
		};

	}
}
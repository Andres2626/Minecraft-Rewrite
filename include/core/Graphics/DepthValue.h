#pragma once

namespace MC 
{
	namespace Graphics 
	{
		/* Values stoled from glad/glad.h */
		enum class DepthValue : u32t
		{
			NEVER = 0x200,
			LESS,
			EQUAL,
			LEQUAL,
			GREATHER,
			NOTEQUAL,
			GEQUAL,
			ALWAYS
		};

	}
}
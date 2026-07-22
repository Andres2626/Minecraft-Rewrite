#include "Graphics/Buffers/VertexTraits.h"

#include <gfx/glad.h>

namespace MC 
{
	namespace Graphics
	{
		static u32t GLTypes[] = {
			GL_FLOAT,
			GL_INT,
			GL_UNSIGNED_INT,
			GL_SHORT,
			GL_UNSIGNED_SHORT,
			GL_BYTE,
			GL_UNSIGNED_BYTE,

			/* vector types */
			GL_FLOAT,
			GL_FLOAT,
			GL_FLOAT
		};

		u32t VertexType2GL(VertexType type)
		{	
			return GLTypes[(u32t)type];
		}
	}
}
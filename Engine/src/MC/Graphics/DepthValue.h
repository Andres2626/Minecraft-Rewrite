#pragma once
#include <glad/glad.h>

namespace MC {
	namespace Graphics {

		enum class DepthValue {
			ALWAYS = GL_ALWAYS,
			NEVER = GL_NEVER,
			LESS = GL_LESS,
			EQUAL = GL_EQUAL,
			LEQUAL = GL_LEQUAL,
			GREATHER = GL_GREATER,
			NOTEQUAL = GL_NOTEQUAL,
			GEQUAL = GL_GEQUAL
		};

	}
}
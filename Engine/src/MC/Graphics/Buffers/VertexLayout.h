#pragma once
#include "common.h"

#include <glad/glad.h>

namespace MC {
	namespace Graphics {

		struct RD_API VertexAttrib {
			GLuint index; // shader bit (view Shaderflags.h)
			GLint size; // dimension of data
			GLenum type; // type of data
			GLsizei stride;
			const void* offset;
			GLboolean normalized;
		};

		class RD_API VertexLayout {
		private:
			std::vector<VertexAttrib> m_Attribs;
		public:
			VertexLayout() = default;
			~VertexLayout() = default;
		public:
			void AddAttribute(GLuint index, GLint size, GLenum type, GLsizei stride, const void* offset, GLboolean normalized = GL_FALSE);
		public:
			void Init() const;
		};

	}
}
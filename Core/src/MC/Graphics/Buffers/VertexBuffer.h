#pragma once
#include "common.h"

#include <glad/glad.h>

namespace MC {
	namespace Graphics {

		class RD_API VertexBuffer {
		private:
			GLuint m_ID, m_Size;
		public:
			VertexBuffer();
			~VertexBuffer();
		public:
			/* build vertex buffer from vertices */
			void Build(GLuint size, const void* data);
		public:
			void Bind() const;
			void Unbind() const;
		public:
			inline GLuint GetId() { return this->m_ID; }
		};

	}
}

#pragma once
#include "common.h"

#include <glad/glad.h>

namespace MC {
	namespace Graphics {

		class MC_API IndexBuffer {
		private:
			GLuint m_ID, m_Size;
		public:
			IndexBuffer();
			~IndexBuffer();
		public:
			void Build(GLuint size, const void* data);
		public:
			void Bind() const;
			void Unbind() const;
		public:
			inline GLuint GetId() { return this->m_ID; }
			inline GLuint GetSize() { return this->m_Size; }
		};

	}
}
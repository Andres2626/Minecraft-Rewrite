#pragma once
#include "common.h"
#include "VertexLayout.h"

#include <glad/glad.h>

namespace MC {
	namespace Graphics {
		
		class RD_API VertexArray {
		private:
			GLuint m_ID;
		public:
			VertexArray();
			~VertexArray();
		public:
			void Bind() const;
			void Unbind() const;
		public:
			void Link(const VertexLayout& layout) const;
		public:
			inline GLuint GetId() { return this->m_ID; }
		};

	}
}
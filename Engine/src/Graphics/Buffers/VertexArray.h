#pragma once

#include <glad/glad.h>

#include "VertexLayout.h"

namespace MC {
	namespace Graphics {
		
		class VertexArray {
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
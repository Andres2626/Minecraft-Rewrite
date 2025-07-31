#include "VertexArray.h"

namespace MC {
	namespace Graphics {

		VertexArray::VertexArray() 
			: m_ID(0)
		{ 
			glGenVertexArrays(1, &this->m_ID);
			this->Bind();
		}

		VertexArray::~VertexArray() 
		{
			glDeleteVertexArrays(1, &this->m_ID);
		}

		void VertexArray::Bind() const 
		{
			glBindVertexArray(this->m_ID);
		}

		void VertexArray::Unbind() const 
		{
			glBindVertexArray(0);
		}

		void VertexArray::Link(const VertexLayout& layout) const {
			layout.Init();
		}

	}
}
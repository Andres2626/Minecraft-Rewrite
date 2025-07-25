
#include "IndexBuffer.h"

namespace MC {
	namespace Graphics {

		IndexBuffer::IndexBuffer()
			: m_ID(0), m_Size(0)
		{ 
			glGenBuffers(1, &this->m_ID);
		}

		IndexBuffer::~IndexBuffer() 
		{
			glDeleteBuffers(1, &this->m_ID);
		}

		void IndexBuffer::Build(GLuint size, const void* data) 
		{
			this->m_Size = size;

			this->Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}

		void IndexBuffer::Bind() const 
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ID);
		}

		void IndexBuffer::Unbind() const 
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
}
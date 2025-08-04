#include "VertexBuffer.h"

#include <glad/glad.h>

namespace MC 
{
	namespace Graphics 
	{

		VertexBuffer::VertexBuffer()
			: m_ID(0), m_Size(0)
		{ 
			glGenBuffers(1, &m_ID);
		}

		VertexBuffer::~VertexBuffer() 
		{
			glDeleteBuffers(1, &m_ID);
		}

		void VertexBuffer::Build(rd_uint8_t size, const void* data)
		{
			this->m_Size = size;

			this->Bind();
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}

		void VertexBuffer::Bind() const 
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		}

		void VertexBuffer::Unbind() const 
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

	}
}

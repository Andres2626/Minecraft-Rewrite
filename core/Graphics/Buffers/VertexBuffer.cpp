#include "Graphics/Buffers/VertexBuffer.h"

#include <gfx/glad.h>

namespace MC 
{
	namespace Graphics 
	{

		VertexBuffer::VertexBuffer(u32t mode)
			: m_ID(0), m_Size(0), m_Mode(mode)
		{ 
			glGenBuffers(1, &m_ID);
		}

		VertexBuffer::~VertexBuffer() 
		{
			glDeleteBuffers(1, &m_ID);
		}

		void VertexBuffer::Build(uintptr_t size, const void *data)
		{
			m_Size = size;

			Bind();
			glBufferData(GL_ARRAY_BUFFER, size, data, m_Mode);
		}

		void VertexBuffer::Resize(uintptr_t size)
		{
			m_Size = size;
			Bind();
			glBufferData(GL_ARRAY_BUFFER, size, NULL, m_Mode);
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

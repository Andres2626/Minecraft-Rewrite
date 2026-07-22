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

		void VertexBuffer::Update(uintptr_t offset, uintptr_t size, const void* data)
		{
			Bind();
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
		}

		void VertexBuffer::Bind() const 
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		}

		void VertexBuffer::Unbind() const 
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void VertexBuffer::SetVertexLayout(const VertexLayout& layout)
		{
			const auto& attr = layout.GetAttribs();
			for (int i = 0; i < attr.size(); i++) {
				bool isIntegerType = (attr[i].type == GL_UNSIGNED_INT || attr[i].type == GL_INT ||
					attr[i].type == GL_UNSIGNED_SHORT || attr[i].type == GL_SHORT ||
					attr[i].type == GL_UNSIGNED_BYTE || attr[i].type == GL_BYTE);

				if (isIntegerType) {
					glVertexAttribIPointer(attr[i].index,
						attr[i].size,
						attr[i].type,
						attr[i].stride,
						attr[i].offset);
				}
				else {
					glVertexAttribPointer(attr[i].index,
						attr[i].size,
						attr[i].type,
						attr[i].normalized,
						attr[i].stride,
						attr[i].offset);
				}
				glEnableVertexAttribArray(attr[i].index);
				if (attr[i].divisor > 0)
					glVertexAttribDivisor(attr[i].index, attr[i].divisor);
			}
		}

	}
}

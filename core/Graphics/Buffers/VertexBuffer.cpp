#include "mc.h"

#include "Graphics/Buffers/VertexBuffer.h"

#include "Graphics/GL/GL.h"

#define MC_LOG_PREFIX "VertexBuffer"
#include "Log/Log.h"

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
			if (!size || !data) {
				mc_warn("Build() Invalid size or data");
				return;
			}

			m_Size = size;
			glBufferData(GL_ARRAY_BUFFER, size, data, m_Mode);
		}

		void VertexBuffer::Resize(uintptr_t size)
		{
			if (!size) {
				mc_warn("Resize() Invalid size");
				return;
			}

			m_Size = size;
			glBufferData(GL_ARRAY_BUFFER, size, NULL, m_Mode);
		}

		void VertexBuffer::Update(uintptr_t offset, uintptr_t size, const void* data)
		{
			if (!size || !data) {
				mc_warn("Update() Invalid size or data");
				return;
			}

			if (offset > m_Size || size > m_Size - offset) {
				mc_warn("Update() offset out of bounds");
				return;
			}

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

		void VertexBuffer::SetVertexLayout(const VertexLayout &layout)
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

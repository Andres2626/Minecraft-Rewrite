#include "Graphics/Buffers/VertexLayout.h"

#include <gfx/glad.h>

namespace MC 
{
	namespace Graphics
	{

		void VertexLayout::AddAttribute(u32t index, i32t size, u32t type, i32t stride, const void *offset, u8t normalized)
		{
			m_Attribs.push_back({ index, size, type, stride, offset, normalized });
		}

		void VertexLayout::Init() const
		{
			for (int i = 0; i < m_Attribs.size(); i++) {
				glVertexAttribPointer(m_Attribs[i].index,
					                  m_Attribs[i].size,
					                  m_Attribs[i].type,
					                  m_Attribs[i].normalized,
					                  m_Attribs[i].stride,
					                  m_Attribs[i].offset);

				glEnableVertexAttribArray(m_Attribs[i].index);
			}
		}

	}
}
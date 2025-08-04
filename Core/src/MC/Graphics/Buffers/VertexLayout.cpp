#include "VertexLayout.h"

#include <glad/glad.h>

namespace MC 
{
	namespace Graphics
	{

		void VertexLayout::AddAttribute(rd_uint8_t index, rd_int_t size, rd_uint8_t type, rd_int8_t stride, const void* offset, rd_uchar8_t normalized)
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
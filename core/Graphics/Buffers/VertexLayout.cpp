#include "Graphics/Buffers/VertexLayout.h"

#include "Graphics/Shader/Shaderflags.h"

#include <gfx/glad.h>

namespace MC 
{
	namespace Graphics
	{

		void VertexLayout::AddAttribute(u32t index, i32t stride, const void *offset, u8t normalized)
		{
			i32t size = 3;
			switch (index)
			{
			case SHADER_VERTEX_BIT:
				size = 3;
				break;
			case SHADER_COLOR_BIT:
				size = 3;
				break;
			case SHADER_TEX_BIT:
				size = 2;
				break;
			case SHADER_BRIG_BIT:
				size = 1;
				break;
			}

			m_Attribs.push_back({ index, size, GL_FLOAT, stride, offset, normalized });
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
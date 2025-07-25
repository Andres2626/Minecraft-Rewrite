
#include "VertexLayout.h"

namespace MC {
	namespace Graphics {
	
		void VertexLayout::AddAttribute(GLuint index, GLint size, GLenum type, GLsizei stride, const void* offset, GLboolean normalized) 
		{
			this->m_Attribs.push_back({ index, size, type, stride, offset, normalized });
		}

		void VertexLayout::Init() const 
		{
			for (int i = 0; i < this->m_Attribs.size(); i++) {
				glVertexAttribPointer(this->m_Attribs[i].index, 
					this->m_Attribs[i].size, 
					this->m_Attribs[i].type, 
					this->m_Attribs[i].normalized, 
					this->m_Attribs[i].stride, 
					this->m_Attribs[i].offset);

				glEnableVertexAttribArray(this->m_Attribs[i].index);
			}
		}
	}
}
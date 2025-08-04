#include "Renderer.h"
#include "BlendValue.h"

#include <glad/glad.h>

namespace MC 
{
	namespace Graphics 
	{

		void Renderer::Enable(rd_uint8_t func) 
		{
			glEnable(func);
		}

		void Renderer::Disable(rd_uint8_t func) 
		{
			glDisable(func);
		}

		void Renderer::ClearColor(Math::vec3 color) 
		{
			glClearColor(color.x, color.y, color.z, 1.0f);
		}

		void Renderer::Viewport(Math::ivec2 pos, Math::ivec2 size) 
		{
			glViewport(pos.x, pos.y, size.x, size.y);
		}

		void Renderer::DepthFunc(const DepthValue& val)
		{
			glDepthFunc((GLenum)val);
		}

		void Renderer::BlendFunc(const BlendValue& sfactor, const BlendValue& dfactor)
		{
			glBlendFunc((GLenum)sfactor, (GLenum)dfactor);
		}

		void Renderer::DrawArrays(rd_uint8_t mode, rd_uint8_t size)
		{
			glDrawArrays(mode, 0, size);
		}

		void Renderer::DrawElements(rd_uint8_t mode, rd_uint8_t size)
		{
			glDrawElements(mode, size, GL_UNSIGNED_INT, 0);
		}

	}
}

#include "Graphics/Renderer.h"
#include "Graphics/BlendValue.h"

#include <gfx/glad.h>

namespace MC 
{
	namespace Graphics 
	{

		void Renderer::Enable(mc_u8 func)
		{
			glEnable(func);
		}

		void Renderer::Disable(mc_u8 func)
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

		void Renderer::DrawArrays(mc_u8 mode, mc_u8 size)
		{
			glDrawArrays(mode, 0, size);
		}

		void Renderer::DrawElements(mc_u8 mode, mc_u8 size)
		{
			glDrawElements(mode, size, GL_UNSIGNED_INT, 0);
		}

	}
}

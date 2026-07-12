#include "Graphics/Renderer.h"
#include "Graphics/BlendValue.h"

#include <gfx/glad.h>

namespace MC 
{
	namespace Graphics 
	{
		void Renderer::Enable(u32t func)
		{
			glEnable(func);
		}

		void Renderer::Disable(u32t func)
		{
			glDisable(func);
		}

		void Renderer::ClearColor(const Math::vec3 &color) 
		{
			glClearColor(color.x, color.y, color.z, 1.0f);
		}

		void Renderer::Viewport(const Math::ivec2 &pos, const Math::ivec2 &size) 
		{
			glViewport(pos.x, pos.y, size.x, size.y);
		}

		void Renderer::DepthFunc(const DepthValue &val)
		{
			glDepthFunc((GLenum)val);
		}

		void Renderer::BlendFunc(const BlendValue &sfactor, const BlendValue &dfactor)
		{
			glBlendFunc((GLenum)sfactor, (GLenum)dfactor);
		}

		void Renderer::CullFace(u32t mode)
		{
			glCullFace(mode);
		}

		void Renderer::FrontFace(u32t mode)
		{
			glFrontFace(mode);
		}

		void Renderer::DrawArrays(u32t mode, i32t size)
		{
			glDrawArrays(mode, 0, size);
		}

		void Renderer::DrawElements(u32t mode, i32t size)
		{
			glDrawElements(mode, size, GL_UNSIGNED_INT, NULL);
		}

		void Renderer::DrawElements(u32t mode, i32t size, const void *indices)
		{
			glDrawElements(mode, size, GL_UNSIGNED_INT, indices);
		}

		void Renderer::DrawElementsInstanced(u32t mode, i32t count, const void* indices, i32t icount)
		{
			glDrawElementsInstanced(mode, count, GL_UNSIGNED_INT, indices, icount);
		}

		void Renderer::Clear(u32t mask)
		{
			glClear(mask);
		}
	}
}

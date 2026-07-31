
#include "Graphics/Renderer.h"

#include "Graphics/BlendValue.h"
#include "Graphics/GL/GL.h"

#include "Log/Log.h"

namespace MC 
{
	namespace Graphics 
	{
        void Renderer::Init()
        {
            mc_warn("graphic driver version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
			mc_warn("driver vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
			mc_warn("driver name: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
            mc_warn("shader version: {}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
        }
        
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

		void Renderer::DrawArrays(u32t mode, size_t size)
		{
			glDrawArrays(mode, 0, (GLsizei)size);
		}

		void Renderer::DrawElements(u32t mode, size_t size)
		{
			glDrawElements(mode, (GLsizei)size, GL_UNSIGNED_INT, NULL);
		}

		void Renderer::DrawElements(u32t mode, size_t size, const void *indices)
		{
			glDrawElements(mode, (GLsizei)size, GL_UNSIGNED_INT, indices);
		}

		void Renderer::DrawElementsInstanced(u32t mode, size_t count, const void* indices, size_t icount)
		{
			glDrawElementsInstanced(mode, (GLsizei)count, GL_UNSIGNED_INT, indices, (GLsizei)icount);
		}

		void Renderer::Clear(u32t mask)
		{
			glClear(mask);
		}
	}
}

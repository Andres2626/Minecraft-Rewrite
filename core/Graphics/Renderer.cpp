#include "mc.h"

#include "Graphics/Renderer.h"

#include "Graphics/BlendValue.h"
#include "Graphics/GL/GL.h"

#include "Log/Log.h"
#include "Utils/Util.h"


namespace MC 
{
	namespace Graphics 
	{
		static RendererCapabilities s_Capabilities;

		static bool s_Init = false;

		static const char *GLSLVersion()
		{
			return (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		}

		static bool SupportsVFShader()
		{
#if defined (MC_PLATFORM_WEB)
            return true;
#else
			return GLAD_GL_VERSION_2_0;
#endif /* MC_PLATFORM_WEB */
		}

		static bool SupportsVBO()
		{
#if defined (MC_PLATFORM_WEB)
            return true;
#else
			return GLAD_GL_VERSION_1_5 || GLAD_GL_ARB_vertex_buffer_object;
#endif /* MC_PLATFORM_WEB */
        
		}

		static bool SupportsVAO()
		{
#if defined (MC_PLATFORM_WEB)
            return true;
#else            
			return GLAD_GL_VERSION_3_0 || GLAD_GL_ARB_vertex_array_object;
#endif /* MC_PLATFORM_WEB */        
		}

		static bool SupportsInstancedDraw()
		{
#if defined (MC_PLATFORM_WEB)
            return true;
#else
			return GLAD_GL_VERSION_3_1 || GLAD_GL_ARB_draw_instanced;
#endif /* MC_PLATFORM_WEB */        
		}

		static bool SupportsInstancedAttribs()
		{
#if defined (MC_PLATFORM_WEB)
            return true;
#else
			return GLAD_GL_VERSION_3_3 || GLAD_GL_ARB_instanced_arrays;
#endif /* MC_PLATFORM_WEB */        
		}

		static void CheckInitDbg()
		{
			mc_assert(s_Init, "renderer is not initialized");
		}

        void Renderer::Init()
        {
			mc_assert(!s_Init, "renderer is already initialized");
			
			s_Capabilities.version = (const char*)glGetString(GL_VERSION);
			s_Capabilities.vendor = (const char*)glGetString(GL_VENDOR);
			s_Capabilities.renderer = (const char*)glGetString(GL_RENDERER);
			s_Capabilities.shaderVer = GLSLVersion();
			s_Capabilities.supportVFShader = SupportsVFShader();
			s_Capabilities.supportVBO = SupportsVBO();
			s_Capabilities.supportVAO = SupportsVAO();
			s_Capabilities.supportInstancedDraw = SupportsInstancedDraw();
			s_Capabilities.supportInstancedAttribs = SupportsInstancedAttribs();

			mc_warn("Renderer Info");
			mc_warn("Renderer: OpenGL");
            mc_warn("Version: {}", s_Capabilities.version);
			mc_warn("Vendor: {}", s_Capabilities.vendor);
			mc_warn("Name: {}", s_Capabilities.renderer);

#if defined (_DEBUG)
			mc_warn("Renderer capabilities");
			mc_warn("Shading languaje version: {}", s_Capabilities.shaderVer);
			mc_warn("Vertex and Fragment shader: {}", s_Capabilities.supportVFShader);
			mc_warn("VBO: {}", s_Capabilities.supportVBO);
			mc_warn("VAO: {}", s_Capabilities.supportVAO);
			mc_warn("Instanced draw: {}", s_Capabilities.supportInstancedDraw);
			mc_warn("Instanced attribs: {}", s_Capabilities.supportInstancedAttribs);
#endif

			s_Init = true;
        }
        
		void Renderer::Enable(u32t func)
		{
			if (s_Init)
				glEnable(func);
		}

		void Renderer::Disable(u32t func)
		{
			if (s_Init)
				glDisable(func);
		}

		void Renderer::ClearColor(const Math::vec3 &color) 
		{
			if (s_Init)
				glClearColor(color.x, color.y, color.z, 1.0f);
		}

		void Renderer::Viewport(const Math::ivec2 &pos, const Math::ivec2 &size) 
		{
			if (s_Init)
				glViewport(pos.x, pos.y, size.x, size.y);
		}

		void Renderer::DepthFunc(const DepthValue &val)
		{
			if (s_Init)
				glDepthFunc((GLenum)val);
		}

		void Renderer::BlendFunc(const BlendValue &sfactor, const BlendValue &dfactor)
		{
			if (s_Init)
				glBlendFunc((GLenum)sfactor, (GLenum)dfactor);
		}

		void Renderer::CullFace(u32t mode)
		{
			if (s_Init)
				glCullFace(mode);
		}

		void Renderer::FrontFace(u32t mode)
		{
			if (s_Init)
				glFrontFace(mode);
		}

		void Renderer::DrawArrays(u32t mode, size_t size)
		{
			if (s_Init)
				glDrawArrays(mode, 0, (GLsizei)size);
		}

		void Renderer::DrawElements(u32t mode, size_t size)
		{
			if (s_Init)
				glDrawElements(mode, (GLsizei)size, GL_UNSIGNED_INT, NULL);
		}

		void Renderer::DrawElements(u32t mode, size_t size, const void *indices)
		{
			if (s_Init)
				glDrawElements(mode, (GLsizei)size, GL_UNSIGNED_INT, indices);
		}

		void Renderer::DrawElementsInstanced(u32t mode, size_t count, const void* indices, size_t icount)
		{
			if (!s_Init)
				return;

			if (s_Capabilities.supportInstancedDraw)
				glDrawElementsInstanced(mode, (GLsizei)count, GL_UNSIGNED_INT, indices, (GLsizei)icount);
		}

		void Renderer::Clear(u32t mask)
		{
			if (s_Init)
				glClear(mask);
		}

		RendererCapabilities &Renderer::GetCapabilities()
		{
			return s_Capabilities;
		}
	}
}

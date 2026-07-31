
#include "Graphics/GL/GLContext.h"

#include "Graphics/GL/GL.h"

#include <GLFW/glfw3.h>

namespace MC 
{
	namespace Graphics 
	{
		namespace GL 
		{
			bool Context::Init(void *win)
			{
				glfwMakeContextCurrent((GLFWwindow*)win);

#ifndef MC_PLATFORM_WEB
				bool glad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
				if (!glad)
					return false;
#endif /* !MC_PLATFORM_WEB */

				return true;
			}
		}
    }
}


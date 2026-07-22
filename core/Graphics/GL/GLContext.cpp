#include "Graphics/GL/GLContext.h"

#include <gfx/glad.h>
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

				bool glad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
				if (!glad)
					return false;

				return true;
			}
		}
    }
}


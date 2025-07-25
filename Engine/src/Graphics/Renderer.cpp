#include <glad/glad.h>
#include "Renderer.h"

namespace MC {
	namespace Graphics {

		void Renderer::Enable(unsigned int func) 
		{
			glEnable(func);
		}

		void Renderer::Disable(unsigned int func) 
		{
			glDisable(func);
		}

		void Renderer::ClearColor(glm::vec3 color) 
		{
			glClearColor(color.x, color.y, color.z, 1.0f);
		}

		void Renderer::Viewport(glm::ivec2 pos, glm::ivec2 size) 
		{
			glViewport(pos.x, pos.y, size.x, size.y);
		}

	}
}

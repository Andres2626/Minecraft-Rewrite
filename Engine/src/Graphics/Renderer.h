#pragma once
#include <glm/glm.hpp>

namespace MC {
	namespace Graphics {

		class Renderer {
		public:
			static void Enable(unsigned int func);
			static void Disable(unsigned int func);
		public:
			static void ClearColor(glm::vec3 color);
			static void Viewport(glm::ivec2 pos, glm::ivec2 size);
		};

	}
}
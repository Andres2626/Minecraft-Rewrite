#pragma once
#include "BlendValue.h"
#include "DepthValue.h"

#include <glm/glm.hpp>

namespace MC {
	namespace Graphics {

		class RD_API Renderer {
		public:
			static void Enable(unsigned int func);
			static void Disable(unsigned int func);
		public:
			static void ClearColor(glm::vec3 color);
			static void Viewport(glm::ivec2 pos, glm::ivec2 size);
		public:
			static void DepthFunc(const DepthValue& val);
			static void BlendFunc(const BlendValue& sfactor, const BlendValue& dfactor);
		public:
			static void DrawArrays(unsigned int mode, unsigned int size);
			static void DrawElements(unsigned int mode, unsigned int size);
		};

	}
}
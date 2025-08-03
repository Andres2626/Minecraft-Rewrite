#pragma once
#include "BlendValue.h"
#include "DepthValue.h"

#include "Math/Math.h"

namespace MC {
	namespace Graphics {

		class RD_API Renderer {
		public:
			static void Enable(unsigned int func);
			static void Disable(unsigned int func);
		public:
			static void ClearColor(Math::vec3 color);
			static void Viewport(Math::ivec2 pos, Math::ivec2 size);
		public:
			static void DepthFunc(const DepthValue& val);
			static void BlendFunc(const BlendValue& sfactor, const BlendValue& dfactor);
		public:
			static void DrawArrays(unsigned int mode, unsigned int size);
			static void DrawElements(unsigned int mode, unsigned int size);
		};

	}
}
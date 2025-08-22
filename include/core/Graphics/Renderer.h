#pragma once
#include "common.h"

#include "BlendValue.h"
#include "DepthValue.h"

#include "Math/Math.h"

namespace MC 
{
	namespace Graphics 
	{

		class MC_API Renderer 
		{
		public:
			static void Enable(mc_u8 func);
			static void Disable(mc_u8 func);
		public:
			static void ClearColor(Math::vec3 color);
			static void Viewport(Math::ivec2 pos, Math::ivec2 size);
		public:
			static void DepthFunc(const DepthValue& val);
			static void BlendFunc(const BlendValue& sfactor, const BlendValue& dfactor);
		public:
			static void DrawArrays(mc_u8 mode, mc_u8 size);
			static void DrawElements(mc_u8 mode, mc_u8 size);
		};

	}
}
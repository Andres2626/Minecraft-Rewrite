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
			static void Enable(rd_uint8_t func);
			static void Disable(rd_uint8_t func);
		public:
			static void ClearColor(Math::vec3 color);
			static void Viewport(Math::ivec2 pos, Math::ivec2 size);
		public:
			static void DepthFunc(const DepthValue& val);
			static void BlendFunc(const BlendValue& sfactor, const BlendValue& dfactor);
		public:
			static void DrawArrays(rd_uint8_t mode, rd_uint8_t size);
			static void DrawElements(rd_uint8_t mode, rd_uint8_t size);
		};

	}
}
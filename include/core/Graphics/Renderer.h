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
			static void Enable(u32t func);
			static void Disable(u32t func);
		public:
			static void ClearColor(const Math::vec3 &color);
			static void Viewport(const Math::ivec2 &pos, const Math::ivec2 &size);
		public:
			static void DepthFunc(const DepthValue &val);
			static void BlendFunc(const BlendValue &sfactor, const BlendValue &dfactor);
		public:
			static void CullFace(u32t mode);
			static void FrontFace(u32t mode);
		public:
			static void DrawArrays(u32t mode, i32t size);
			static void DrawElements(u32t mode, i32t size);
			static void DrawElements(u32t mode, i32t size, const void* indices);
		public:
			static void Clear(u32t mode);
		};

	}
}
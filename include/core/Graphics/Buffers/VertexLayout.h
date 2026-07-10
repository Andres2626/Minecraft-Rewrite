#pragma once
#include "common.h"

namespace MC 
{
	namespace Graphics 
	{

		struct MC_API VertexAttrib
		{
			u32t index; // shader bit (view Shaderflags.h)
			i32t size; // dimension of data
			u32t type; // type of data
			i32t stride;
			const void* offset;
			u8t normalized;
		};

		class MC_API VertexLayout 
		{
		private:
			std::vector<VertexAttrib> m_Attribs;
		public:
			VertexLayout() = default;
			~VertexLayout() = default;
		public:
			void AddAttribute(u32t index, i32t stride, const void* offset, u8t normalized = 0);
		public:
			void Init() const;
		};

	}
}
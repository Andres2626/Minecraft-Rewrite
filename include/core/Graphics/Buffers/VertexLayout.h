#pragma once
#include "common.h"

namespace MC 
{
	namespace Graphics 
	{

		struct MC_API VertexAttrib
		{
			mc_u8 index; // shader bit (view Shaderflags.h)
			mc_int size; // dimension of data
			mc_u8 type; // type of data
			mc_i8 stride;
			const void* offset;
			mc_uc8 normalized;
		};

		class MC_API VertexLayout 
		{
		private:
			std::vector<VertexAttrib> m_Attribs;
		public:
			VertexLayout() = default;
			~VertexLayout() = default;
		public:
			void AddAttribute(mc_u8 index, mc_int size, mc_u8 type, mc_i8 stride, const void* offset, mc_uc8 normalized = 0);
		public:
			void Init() const;
		};

	}
}
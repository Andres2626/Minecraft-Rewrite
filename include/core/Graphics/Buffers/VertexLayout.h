#pragma once
#include "common.h"

namespace MC 
{
	namespace Graphics 
	{

		struct MC_API VertexAttrib
		{
			rd_uint8_t index; // shader bit (view Shaderflags.h)
			rd_int_t size; // dimension of data
			rd_uint8_t type; // type of data
			rd_int8_t stride;
			const void* offset;
			rd_uchar8_t normalized;
		};

		class MC_API VertexLayout 
		{
		private:
			std::vector<VertexAttrib> m_Attribs;
		public:
			VertexLayout() = default;
			~VertexLayout() = default;
		public:
			void AddAttribute(rd_uint8_t index, rd_int_t size, rd_uint8_t type, rd_int8_t stride, const void* offset, rd_uchar8_t normalized = 0);
		public:
			void Init() const;
		};

	}
}
#pragma once
#include "common.h"

namespace MC 
{
	namespace Graphics 
	{

		struct MC_API VertexAttrib
		{
			u32t index;
			i32t size;
			u32t type;
			i32t stride;
			const void* offset;
			u8t normalized;
			u32t divisor;
		};

		class MC_API VertexLayout 
		{
		private:
			std::vector<VertexAttrib> m_Attribs;
		public:
			VertexLayout() = default;
			~VertexLayout() = default;
		public:
			void AddAttribute(u32t index, i32t stride, const void* offset, u32t divisor = 0, i32t size = 0, u8t normalized = 0);
		public:
			void Init() const;
		public:
			static void debug(u32t index, i32t pname, i32t* params);
		};

	}
}
#pragma once
#include "common.h"
#include "VertexLayout.h"

namespace MC 
{
	namespace Graphics 
	{
		
		class MC_API VertexArray 
		{
		private:
			rd_uint8_t m_ID;
		public:
			VertexArray();
			~VertexArray();
		public:
			void Bind() const;
			void Unbind() const;
		public:
			void Link(const VertexLayout& layout) const;
		public:
			inline rd_int8_t GetId() { return this->m_ID; }
		};

	}
}
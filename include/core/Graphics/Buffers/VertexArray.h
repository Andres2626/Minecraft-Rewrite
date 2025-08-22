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
			mc_u8 m_ID;
		public:
			VertexArray();
			~VertexArray();
		public:
			void Bind() const;
			void Unbind() const;
		public:
			void Link(const VertexLayout& layout) const;
		public:
			inline mc_u8 GetId() { return this->m_ID; }
		};

	}
}
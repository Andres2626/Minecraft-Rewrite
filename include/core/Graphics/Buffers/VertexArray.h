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
			u32t m_ID;
		public:
			VertexArray();
			~VertexArray();
		public:
			void Bind() const;
			void Unbind() const;
		public:
			void Link(const VertexLayout& layout) const;
		public:
			inline u32t GetId() { return m_ID; }
		};

	}
}
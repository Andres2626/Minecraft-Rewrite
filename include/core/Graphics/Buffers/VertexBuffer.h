#pragma once
#include "common.h"

namespace MC 
{
	namespace Graphics 
	{

		class MC_API VertexBuffer 
		{
		private:
			mc_u8 m_ID;
			size_t m_Size;
		public:
			VertexBuffer();
			~VertexBuffer();
		public:
			/* build vertex buffer from vertices */
			void Build(size_t size, const void* data);
		public:
			void Bind() const;
			void Unbind() const;
		public:
			inline mc_u8 GetId() { return this->m_ID; }
		};

	}
}

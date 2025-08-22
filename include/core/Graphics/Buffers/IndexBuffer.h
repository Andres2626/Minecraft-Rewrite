#pragma once
#include "common.h"

namespace MC 
{
	namespace Graphics 
	{

		class MC_API IndexBuffer 
		{
		private:
			mc_u8 m_ID;
			mc_u8 m_Size;
		public:
			IndexBuffer();
			~IndexBuffer();
		public:
			void Build(mc_u8 size, const void* data);
		public:
			void Bind() const;
			void Unbind() const;
		public:
			inline mc_u8 GetId() { return this->m_ID; }
			inline mc_u8 GetSize() { return this->m_Size; }
		};

	}
}
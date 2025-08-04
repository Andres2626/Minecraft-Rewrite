#pragma once
#include "common.h"

namespace MC 
{
	namespace Graphics 
	{

		class MC_API IndexBuffer 
		{
		private:
			rd_uint8_t m_ID;
			rd_uint8_t m_Size;
		public:
			IndexBuffer();
			~IndexBuffer();
		public:
			void Build(rd_uint8_t size, const void* data);
		public:
			void Bind() const;
			void Unbind() const;
		public:
			inline rd_uint8_t GetId() { return this->m_ID; }
			inline rd_uint8_t GetSize() { return this->m_Size; }
		};

	}
}
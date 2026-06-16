#pragma once
#include "common.h"

namespace MC 
{
	namespace Graphics 
	{

		class MC_API IndexBuffer 
		{
		private:
			u32t m_ID;
			uintptr_t m_Size;
		public:
			IndexBuffer();
			~IndexBuffer();
		public:
			void Build(uintptr_t size, const void *data);
		public:
			void Bind() const;
			void Unbind() const;
		public:
			inline u32t GetId() { return m_ID; }
			inline uintptr_t GetSize() { return m_Size; }
		};

	}
}
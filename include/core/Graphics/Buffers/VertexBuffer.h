#pragma once
#include "common.h"

namespace MC 
{
	namespace Graphics 
	{

		class MC_API VertexBuffer 
		{
		private:
			u32t m_ID;
			uintptr_t m_Size;
			u32t m_Mode;
		public:
			VertexBuffer(u32t mode);
			~VertexBuffer();
		public:
			void Build(uintptr_t size, const void* data);
			void Resize(uintptr_t size);
			void Update(uintptr_t offset, uintptr_t size, const void *data);
		public:
			void Bind() const;
			void Unbind() const;
		public:
			inline u32t GetId() { return m_ID; }
			inline u32t GetMode() { return m_Mode; }
		};

	}
}

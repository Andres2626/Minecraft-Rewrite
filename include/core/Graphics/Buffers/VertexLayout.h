#pragma once

#include "VertexTraits.h"

#include "Log/Log.h"

#include "common.h"

namespace MC 
{
	namespace Graphics 
	{
		struct VertexAttrib
		{
			u32t index;
			i32t size;
			u32t type;
			i32t stride;
			const void* offset;
			u8t normalized;

			/* OpenGL >3.3 instancing */
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
			template <typename T>
			void AddAttribute(u32t index, i32t stride, const void* offset, u32t divisor = 0, bool normalized = 0);
		public:
			const std::vector<VertexAttrib> &GetAttribs() const { return m_Attribs; };
		};

		template<typename T>
		inline void VertexLayout::AddAttribute(u32t index, i32t stride, const void* offset, u32t divisor, bool normalized)
		{
			i32t size = VertexTrait<T>::size;
			u32t type = VertexType2GL(VertexTrait<T>::type);
			m_Attribs.push_back({index, size, type, stride, offset, normalized, divisor});
		}
	}
}
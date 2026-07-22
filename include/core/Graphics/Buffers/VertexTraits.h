#pragma once

#include "Math/Math.h"

#include "common.h"

namespace MC 
{
	namespace Graphics 
	{
		enum class VertexType 
		{
			Float,
			Int32,
			UInt32,
			Int16,
			UInt16,
			Int8,
			UInt8,

			/* vector types */
			Float4,
			Float3,
			Float2,
		};

		template <typename T>
		struct VertexTrait;

		template<>
		struct VertexTrait<float>
		{
			static constexpr VertexType type = VertexType::Float;
			static constexpr i32t size = 1;
		};

		template<>
		struct VertexTrait<i32t>
		{
			static constexpr VertexType type = VertexType::Int32;
			static constexpr i32t size = 1;
		};

		template<>
		struct VertexTrait<u32t>
		{
			static constexpr VertexType type = VertexType::UInt32;
			static constexpr i32t size = 1;
		};

		template<>
		struct VertexTrait<i16t>
		{
			static constexpr VertexType type = VertexType::Int16;
			static constexpr i32t size = 1;
		};

		template<>
		struct VertexTrait<u16t>
		{
			static constexpr VertexType type = VertexType::UInt16;
			static constexpr i32t size = 1;
		};

		template<>
		struct VertexTrait<i8t>
		{
			static constexpr VertexType type = VertexType::Int8;
			static constexpr i32t size = 1;
		};

		template<>
		struct VertexTrait<u8t>
		{
			static constexpr VertexType type = VertexType::UInt8;
			static constexpr i32t size = 1;
		};

		template<>
		struct VertexTrait<Math::vec4>
		{
			static constexpr VertexType type = VertexType::Float4;
			static constexpr i32t size = 4;
		};

		template<>
		struct VertexTrait<Math::vec3>
		{
			static constexpr VertexType type = VertexType::Float3;
			static constexpr i32t size = 3;
		};

		template<>
		struct VertexTrait<Math::vec2>
		{
			static constexpr VertexType type = VertexType::Float2;
			static constexpr i32t size = 2;
		};

		u32t MC_API VertexType2GL(VertexType type);
	}
}
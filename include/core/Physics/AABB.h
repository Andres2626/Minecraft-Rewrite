#pragma once
#include "Math/Math.h"

#include "common.h"

/* stoled from rubydung/phys/AABB.java */

namespace MC 
{
	namespace Physics 
	{

		class MC_API AABB {
		public:
			Math::vec3 p0;
			Math::vec3 p1;
			float epsilon;
		public:
			AABB() = default;
			AABB(const Math::vec3& p0, const Math::vec3& p1);
			~AABB();
		public:
			AABB Expand(const Math::vec3& pos);
			AABB Grow(const Math::vec3& pos);
		public:
			float ClipXCollide(const AABB& other, float x);
			float ClipYCollide(const AABB& other, float y);
			float ClipZCollide(const AABB& other, float z);
			bool Intersects(const AABB& other);
			void Move(const Math::vec3& pos);
		};

	}
}

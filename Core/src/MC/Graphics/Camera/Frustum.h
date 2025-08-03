#pragma once
#include "common.h"
#include "Math/Math.h"
#include "Physics/AABB.h"

namespace MC {
	namespace Graphics {

		class RD_API Frustum {
		protected:
			/* the frustum array */
			float ft[6][4];
		public:
			Frustum() = default;
			~Frustum() = default;
		public:
			void Calculate(Math::mat4 proj, Math::mat4 view);
		public:
			bool PointInside(Math::vec3 pos);
			bool CubeInside(Physics::AABB& aabb);
			bool CubeFullyInside(Physics::AABB& aabb);
			bool SphereInside(Math::vec3 pos, float r);
		};

	}
}

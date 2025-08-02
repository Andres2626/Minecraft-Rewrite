#pragma once
#include "common.h"
#include "Physics/AABB.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
			void Calculate(glm::mat4 proj, glm::mat4 view);
		public:
			bool PointInside(glm::vec3 pos);
			bool CubeInside(Physics::AABB& aabb);
			bool CubeFullyInside(Physics::AABB& aabb);
			bool SphereInside(glm::vec3 pos, float r);
		};

	}
}

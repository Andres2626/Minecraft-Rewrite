#pragma once
#include "common.h"
#include "Frustum.h"
#include "Physics/AABB.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace MC {
	namespace Graphics {

		class Camera {
		protected:
			Frustum* fr;
		public:
			glm::vec3 pos;
			glm::vec2 rot;
			glm::vec3 front, up, right;
			float aspect;
			float near, far;
			float fov;
		public:
			/* build camera */
			Camera(glm::vec3 pos);
			~Camera();
		public:
			/* Detect if box is in frustum (not fully) */
			bool InFrustum(Physics::AABB& box);

			/* Update vectors and recalculate frustum. */
			void Update();
		public:
			/* calculate view matrix */
			glm::mat4 GetView();

			/* calculate proj matrix */
			glm::mat4 GetProj();
		};

	}
}
#pragma once
#include "common.h"
#include "Frustum.h"
#include "Math/Math.h"
#include "Physics/AABB.h"

namespace MC {
	namespace Graphics {

		class RD_API Camera {
		protected:
			Frustum* fr;
		public:
			Math::vec3 pos;
			Math::vec2 rot;
			Math::vec3 front, up, right;
			float aspect;
			float near, far;
			float fov;
		public:
			/* build camera */
			Camera(const Math::vec3& pos);
			~Camera();
		public:
			/* Detect if box is in frustum (not fully) */
			bool InFrustum(Physics::AABB box);

			/* Update vectors and recalculate frustum. */
			void Update();
		public:
			/* calculate view matrix */
			Math::mat4 GetView();

			/* calculate proj matrix */
			Math::mat4 GetProj();
		};

	}
}
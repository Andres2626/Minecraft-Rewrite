#pragma once
#include "common.h"
#include "Frustum.h"
#include "Math/Math.h"
#include "Physics/AABB.h"

namespace MC 
{
	namespace Graphics 
	{

		class MC_API Camera
		{
		protected:
			std::unique_ptr<Frustum> m_Frustum;
		public:
			Math::vec3 pos;
			Math::vec2 rot;
			Math::vec3 front;
			Math::vec3 up;
			Math::vec3 right;
		public:
			Camera();
			~Camera() = default;
		public:
			Math::mat4 GetView() const;
			bool InFrustum(Physics::AABB& box);
		};
	}
}
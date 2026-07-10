#include "Graphics/Camera/Camera.h"

namespace MC 
{
	namespace Graphics 
	{

		Camera::Camera()
		{

		}

		Math::mat4 Camera::GetView() const
		{
			return Math::lookAt(pos, pos + front, up);
		}

		bool Camera::InFrustum(Physics::AABB& box)
		{
			return m_Frustum->CubeInside(box);
		}

	}
}
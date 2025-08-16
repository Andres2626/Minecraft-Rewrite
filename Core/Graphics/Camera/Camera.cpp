#include "Graphics/Camera/Camera.h"

namespace MC {
	namespace Graphics {

		Camera::Camera(const Math::vec3& pos)
			: pos(pos), aspect(0.0f), near(0.0f), far(0.0f), fov(0.0f), rot(0.0f), front(0.0f)
		{
			right = Math::vec3(0.0f);
			up = Math::vec3(0.0f);
			fr = new Frustum();
		}

		Camera::~Camera() 
		{
			if (fr)
				delete fr;
		}

		bool Camera::InFrustum(Physics::AABB box) 
		{
			return fr->CubeInside(box);
		}

		void Camera::Update() 
		{
			front = Math::normalize(Math::vec3(cos(Math::radians(rot.x)) * cos(Math::radians(rot.y)), 
				                          sin(Math::radians(rot.y)), 
				                          sin(Math::radians(rot.x)) * cos(Math::radians(rot.y))));

			right = Math::normalize(Math::cross(front, { 0.0f, 1.0f, 0.0f }));
			up = Math::normalize(Math::cross(right, front));

			/* Calculate frustum vectors */
			fr->Calculate(GetProj(), GetView());
		}

		Math::mat4 Camera::GetView() 
		{
			return Math::lookAt(pos, pos + front, up);
		}

		Math::mat4 Camera::GetProj() 
		{
			return Math::perspective(fov, aspect, near, far);
		}

	}
}
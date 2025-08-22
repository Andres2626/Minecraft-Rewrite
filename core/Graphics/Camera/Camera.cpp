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
			if (this->fr)
				delete fr;
		}

		bool Camera::InFrustum(Physics::AABB box) 
		{
			return this->fr->CubeInside(box);
		}

		void Camera::Update() 
		{
			this->front = Math::normalize(Math::vec3(cos(Math::radians(this->rot.x)) * cos(Math::radians(this->rot.y)), 
				                          sin(Math::radians(this->rot.y)), 
				                          sin(Math::radians(this->rot.x)) * cos(Math::radians(this->rot.y))));

			this->right = Math::normalize(Math::cross(this->front, { 0.0f, 1.0f, 0.0f }));
			this->up = Math::normalize(Math::cross(this->right, this->front));

			/* Calculate frustum vectors */
			this->fr->Calculate(this->GetProj(), this->GetView());
		}

		Math::mat4 Camera::GetView() 
		{
			return Math::lookAt(this->pos, this->pos + this->front, this->up);
		}

		Math::mat4 Camera::GetProj() 
		{
			return Math::perspective(this->fov, this->aspect, this->near, this->far);
		}

	}
}
#include "Camera.h"

namespace MC {
	namespace Graphics {

		Camera::Camera(glm::vec3 pos)
			: pos(pos), aspect(0.0f), near(0.0f), far(0.0f), fov(0.0f), rot(0.0f), front(0.0f)
		{
			right = glm::vec3(0.0f);
			this->up = glm::vec3(0.0f);
			this->fr = new Frustum();
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
			glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

			this->front = glm::normalize(glm::vec3(cos(glm::radians(this->rot.x)) * cos(glm::radians(this->rot.y)), sin(glm::radians(this->rot.y)), sin(glm::radians(this->rot.x)) * cos(glm::radians(this->rot.y))));
			this->right = glm::normalize(glm::cross(this->front, worldUp));
			this->up = glm::normalize(glm::cross(this->right, this->front));
			this->fr->Calculate(this->GetProj(), this->GetView());
		}

		glm::mat4 Camera::GetView() 
		{
			return glm::lookAt(this->pos, this->pos + this->front, this->up);
		}

		glm::mat4 Camera::GetProj() 
		{
			return glm::perspective(this->fov, this->aspect, this->near, this->far);
		}

	}
}
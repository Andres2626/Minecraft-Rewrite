#include "Graphics/Camera/PerspectiveCamera.h"

namespace MC
{
	namespace Graphics
	{
		PerspectiveCamera::PerspectiveCamera(Math::vec3 pos)
			: Camera(), aspect(0.0f), near(0.0f), far(0.0f), fov(0.0f)
		{
			m_Frustum = std::make_unique<Frustum>();
			this->pos = pos;
			right = Math::vec3(0.0f);
			up = Math::vec3(0.0f);
			rot = Math::vec3(0.0f);
			front = Math::vec3(0.0f);
		}

		PerspectiveCamera::~PerspectiveCamera()
		{

		}

		void PerspectiveCamera::Update()
		{
			front = Math::normalize(Math::vec3(cos(Math::radians(rot.x)) * cos(Math::radians(rot.y)),
				                   sin(Math::radians(rot.y)),
				                   sin(Math::radians(rot.x)) * cos(Math::radians(rot.y))));

			right = Math::normalize(Math::cross(front, { 0.0f, 1.0f, 0.0f }));
			up = Math::normalize(Math::cross(right, front));

			m_Frustum->Calculate(GetProjection(), GetView());
		}

		Math::mat4 PerspectiveCamera::GetProjection() const
		{
			return Math::perspective(fov, aspect, near, far);
		}
	}
}
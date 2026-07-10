#pragma once

#include "Camera.h"

namespace MC
{
	namespace Graphics
	{
		class MC_API PerspectiveCamera : public Camera 
		{		
		public:
			float aspect;
			float near;
			float far;
			float fov;
		public:
			PerspectiveCamera(Math::vec3 pos);
			~PerspectiveCamera();
		public:
			void Update();
		public:
			Math::mat4 GetProjection() const;
		};
	}
}
#pragma once

#include "Camera.h"

namespace MC
{
	namespace Graphics
	{
		class MC_API OrthographicCamera : public Camera
		{
		public:
			float zright;
			float zleft;
			float zbottom;
			float ztop;
			float znear;
			float zfar;
		public:
			OrthographicCamera(float left, float right, float bottom, float top);
			OrthographicCamera(float left, float right, float bottom, float top, float near, float far);
			~OrthographicCamera();
		public:
			void Update();
		public:
			Math::mat4 GetProjection() const;
		};
	}
}
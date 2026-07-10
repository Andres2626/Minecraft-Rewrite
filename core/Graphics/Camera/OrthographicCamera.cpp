#include "Graphics/Camera/OrthographicCamera.h"

namespace MC
{
	namespace Graphics
	{
		OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
			: Camera(), zright(right), zleft(left), zbottom(bottom), ztop(top)
		{

		}

		OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
			: Camera(), zright(right), zleft(left), zbottom(bottom), ztop(top), znear(near), zfar(far)
		{

		}

		OrthographicCamera::~OrthographicCamera()
		{

		}

		void OrthographicCamera::Update()
		{
			front = { 0.0f, 0.0f, -1.0f };
			right = { 1.0f, 0.0f, 0.0f };
			up = { 0.0f, 1.0f, 0.0f };
		}

		Math::mat4 OrthographicCamera::GetProjection() const
		{
			return Math::ortho(zleft, zright, zbottom, ztop, znear, zfar);
		}
	}
}
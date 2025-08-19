#include "Graphics/Camera/Ray.h"

namespace MC {
	namespace Graphics {

		Math::vec3 Ray::GetRay(float step, Math::vec3 pos, Math::vec3 dir) 
		{
			/* calculate parametric equation of the camera */
			return pos + dir * step;
		}
	}
}
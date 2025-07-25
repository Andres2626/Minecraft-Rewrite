
#include "Ray.h"

namespace MC {
	namespace Graphics {

		glm::vec3 Ray::GetRay(float step, glm::vec3 pos, glm::vec3 dir) 
		{
			/* calculate parametric equation of the camera */
			return pos + dir * step;
		}
	}
}
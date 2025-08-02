#pragma once
#include "common.h"
#include "Camera.h"

namespace MC {
	namespace Graphics {

		class RD_API Ray {
		public:
			Ray() = default;
			~Ray() = default;
		public:
			/* 
			  The following is obtained by means of the parametric equations of the rect 
			  f(t) = pos + dir * t where t is the independent term, pos is the camera 
			  position and dir is the camera direction. To use the ray it is 
			  necessary to configure the maximum size of the ray segment and step (t)
			  is the distance at which the position to which the ray points is 
			  evaluated by means of a for loop.

			  returns the position of the ray points to.

			  TODO: improve this!
			*/
			glm::vec3 GetRay(float t, glm::vec3 pos, glm::vec3 dir);
		};


	}
}
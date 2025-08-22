#pragma once
#include <Math/Math.h>

using namespace MC;
using namespace Math;

struct Hitresult 
{
	/* impact block */
	ivec3 block;

	/* normal of the face (impacted block) */
	ivec3 face;

	/* point of ray collition */
	vec3 point;

	/* distance block to camera position */
	float dis;
};
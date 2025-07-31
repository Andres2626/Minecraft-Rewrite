#pragma once
#include "common.h"

#include <glm/glm.hpp>

struct Hitresult {
	/* impact block */
	glm::ivec3 block;

	/* normal of the face (impacted block) */
	glm::ivec3 face;

	/* point of ray collition */
	glm::vec3 point;

	/* distance block to camera position */
	float dis;
};
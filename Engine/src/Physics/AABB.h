#pragma once
#include <glm/glm.hpp>	

/* stoled from rubydung/phys/AABB.java */

namespace MC {
	namespace Physics {

		class AABB {
		public:
			glm::vec3 p0;
			glm::vec3 p1;
			float epsilon;
		public:
			AABB() = default;
			AABB(const glm::vec3& p0, const glm::vec3& p1);
			~AABB();
		public:
			AABB Expand(const glm::vec3& pos);
			AABB Grow(const glm::vec3& pos);
		public:
			float ClipXCollide(const AABB& other, float x);
			float ClipYCollide(const AABB& other, float y);
			float ClipZCollide(const AABB& other, float z);
			bool Intersects(const AABB& other);
			void Move(const glm::vec3& pos);
		};

	}
}

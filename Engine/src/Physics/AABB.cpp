#include "AABB.h"

/* stoled from rubydung/phys/AABB.java */

namespace MC {
	namespace Physics {

		AABB::AABB(const glm::vec3& p0, const glm::vec3& p1) 
		{
			this->p0 = p0;
			this->p1 = p1;
			this->epsilon = 0.0f;
		}

		AABB::~AABB() 
		{

		}

		AABB AABB::Expand(const glm::vec3& pos) 
		{
			glm::vec3 _p0 = this->p0;
			glm::vec3 _p1 = this->p1;

			if (pos.x < 0.0f)
				_p0.x += pos.x;
			else
				_p1.x += pos.x;

			if (pos.y < 0.0f)
				_p0.y += pos.y;
			else
				_p1.y += pos.y;

			if (pos.z < 0.0f)
				_p0.z += pos.z;
			else
				_p1.z += pos.z;

			return AABB(_p0, _p1);
		}

		AABB AABB::Grow(const glm::vec3& pos) 
		{
			glm::vec3 _p0 = this->p0 - pos;
			glm::vec3 _p1 = this->p1 + pos;
			return AABB(_p0, _p1);
		}

		float AABB::ClipXCollide(const AABB& other, float x) 
		{
			if (other.p1.y <= this->p0.y || other.p0.y >= this->p1.y)
				return x;

			if (other.p1.z <= this->p0.z || other.p0.z >= this->p1.z)
				return x;

			if (x > 0.0f && other.p1.x <= this->p0.x) {
				float max = this->p0.x - other.p1.x - this->epsilon;
				if (max < x)
					x = max;
			}

			if (x < 0.0f && other.p0.x >= this->p1.x) {
				float max = this->p1.x - other.p0.x + this->epsilon;
				if (max > x)
					x = max;
			}
			return x;
		}

		float AABB::ClipYCollide(const AABB& other, float y) 
		{
			if (other.p1.x <= this->p0.x || other.p0.x >= this->p1.x)
				return y;

			if (other.p1.z <= this->p0.z || other.p0.z >= this->p1.z)
				return y;

			if (y > 0.0f && other.p1.y <= this->p0.y) {
				float max = this->p0.y - other.p1.y - this->epsilon;
				if (max < y)
					y = max;
			}
			if (y < 0.0f && other.p0.y >= this->p1.y) {
				float max = this->p1.y - other.p0.y + this->epsilon;
				if (max > y)
					y = max;
			}
			return y;
		}

		float AABB::ClipZCollide(const AABB& other, float z) 
		{
			if (other.p1.x <= this->p0.x || other.p0.x >= this->p1.x)
				return z;

			if (other.p1.y <= this->p0.y || other.p0.y >= this->p1.y)
				return z;

			if (z > 0.0f && other.p1.z <= this->p0.z) {
				float max = this->p0.z - other.p1.z - this->epsilon;
				if (max < z)
					z = max;
			}
			if (z < 0.0f && other.p0.z >= this->p1.z) {
				float max = this->p1.z - other.p0.z + this->epsilon;
				if (max > z)
					z = max;
			}
			return z;
		}

		bool AABB::Intersects(const AABB& other) 
		{
			bool x0 = other.p1.x > this->p0.x;
			bool x1 = other.p0.x < this->p1.x;
			bool y0 = other.p1.y > this->p0.y;
			bool y1 = other.p0.y < this->p1.y;
			bool z0 = other.p1.z > this->p0.z;
			bool z1 = other.p0.z < this->p1.z;
			return x0 && x1 && y0 && y1 && z0 && z1;
		}

		void AABB::Move(const glm::vec3& pos) 
		{
			this->p0 += pos;
			this->p1 += pos;
		}
	}
}

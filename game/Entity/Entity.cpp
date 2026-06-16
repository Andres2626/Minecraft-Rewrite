#include "Entity/Entity.h"

#include "Level/Level.h"

Entity::Entity(Level &level)
	: m_Level(level)
{
	attr.isGround = false;
	attr.size = vec2(0.3f, 0.9f);
	attr.pos = vec3(0.0f, 0.0f, 0.0f);
	attr.delta = vec3(0.0f, 0.0f, 0.0f);
	attr.rot = vec3(0.0f, 0.0f, 0.0f);
	attr.box = AABB();
	ResetPos();
}

Entity::~Entity()
{

}

void Entity::Move(const vec3& pos)
{
	vec3 oldPos = pos;
	vec3 newPos = pos;

	std::vector<AABB> aabbs = m_Level.GetCubes(attr.box.Expand(pos));

	for (AABB& aabb : aabbs) {
		newPos.x = aabb.ClipXCollide(attr.box, newPos.x);
		newPos.y = aabb.ClipYCollide(attr.box, newPos.y);
		newPos.z = aabb.ClipZCollide(attr.box, newPos.z);
	}

	attr.box.Move(newPos);
	attr.isGround = (oldPos.y != newPos.y) && (oldPos.y < 0.0f);

	vec3 delta = newPos - oldPos;
	if (delta.x != 0.0f)
		attr.delta.x = 0.0f;

	if (delta.y != 0.0f)
		attr.delta.y = 0.0f;

	if (delta.z != 0.0f)
		attr.delta.z = 0.0f;

	attr.pos = { (attr.box.p0.x + attr.box.p1.x) / 2.0f,
				  attr.box.p0.y + attr.heightOffset,
			     (attr.box.p0.z + attr.box.p1.z) / 2.0f };
}

void Entity::MoveRelative(vec2 a, float speed)
{
	float dis = length(a);
	float s = sin(radians(attr.rot.x));
	float c = cos(radians(attr.rot.x));
	if (dis >= 0.01f) {
		dis = speed / sqrt(dis);
		a *= dis;
		attr.delta.x += a.x * c - a.y * s;
		attr.delta.z += a.y * c + a.x * s;
	}
}

void Entity::ResetPos()
{
	vec3 newPos;
	ivec3 Size = m_Level.GetSize();
	newPos.x = (float)(rand() % Size.x + 1);
	newPos.y = (float)(Size.y + 10);
	newPos.z = (float)(rand() % Size.z + 1);
	SetPos(newPos);
}

void Entity::SetPos(const vec3& newPos)
{
	attr.pos = newPos;

	/* Calculate new AABB */
	attr.box = AABB({ newPos.x - attr.size.x, newPos.y - attr.size.y, newPos.z - attr.size.x },
				    { newPos.x + attr.size.x, newPos.y + attr.size.y, newPos.z + attr.size.x });
}

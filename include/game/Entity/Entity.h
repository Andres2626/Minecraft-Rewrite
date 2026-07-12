#pragma once

#include <Physics/AABB.h>
#include <Math/Math.h>
#include <Graphics/Shader/Shader.h>

class Level;

using namespace MC;
using namespace Math;
using namespace Physics;
using namespace Graphics;

struct EntityAttributes
{
	bool isGround;
	bool isDead;
	float heightOffset;
	vec2 size;
	vec3 pos;
	vec3 oldPos;
	vec3 motion;
	vec2 rot;
	AABB box;
};

class Entity
{
protected:
	Level &m_Level;
public:
	EntityAttributes attr;
public:
	Entity(Level& level);
	~Entity();
public:
	virtual void Move(const vec3& pos);
	virtual void MoveRelative(vec2 a, float speed);
	virtual void ResetPos();
	virtual void SetPos(const vec3& newPos);
	virtual void Update() = 0;
};
#pragma once
#include <Graphics/Shader/Shader.h>
#include <Graphics/Camera/Camera.h>

#include <GLFW\glfw3.h>

using namespace MC;
using namespace Graphics;
using namespace Math;
using namespace Physics;

class Level;

class Player {
protected:
	Level* lev;
private:
	bool m_Ground;
	vec2 m_Size;
public:
	Camera cam;
	AABB aabb;
	vec3 pos;
	vec3 movdelta;
public:
	Player(Level* level);
	~Player();
public:
	void Render(Shader* shader);
	void Update();
	void tick();
public:
	void Move(const vec3& pos);
	void MoveRelative(vec2 a, float speed);
	void turn(vec2 mouse);
public:
	void ResetPos();
	void SetPos(const vec3& newPos);
public:
	inline bool GetGround() { return this->m_Ground; }
};
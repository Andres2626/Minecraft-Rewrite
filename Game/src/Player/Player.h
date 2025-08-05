#pragma once
#include <Graphics/Shader/Shader.h>
#include <Graphics/Camera/Camera.h>

#include <GLFW\glfw3.h>

using namespace MC;
using namespace Graphics;
using namespace Math;
using namespace Physics;

class Level;

class Player 
{
protected:
	Level* m_Lev;
private:
	bool m_Ground;
	vec2 m_Size;
	AABB m_Box;
	vec3 m_Pos;
	vec3 m_DMov;
public:
	Camera Cam;
public:
	Player(Level* level);
	~Player();
public:
	void Render();
	void Update();
public:
	void Move(const vec3& pos);
	void MoveRelative(vec2 a, float speed);
	void MouseMove(vec2 pos);
public:
	void ResetPos();
	void SetPos(const vec3& newPos);
public:
	inline bool GetGround() { return m_Ground; }
};
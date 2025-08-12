#pragma once
#include "Hitresult.h"
#include "Selector.h"

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
	Level* m_Level;
private:
	Selector* m_Sel;
	bool m_MouseLeft;
	bool m_MouseRight;
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
private:
	void Move(const vec3& pos);
	void MoveRelative(vec2 a, float speed);
	bool Raycast(const vec3& org, const vec3& dir, Hitresult& ret);
public:
	void MouseMove(vec2 pos);
	void ResetPos();
	void SetPos(const vec3& newPos);
public:
	void Pick(float time, Shader* shader);
public:
	inline bool GetGround() { return m_Ground; }
};
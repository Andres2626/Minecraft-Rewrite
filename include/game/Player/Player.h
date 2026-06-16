#pragma once
#include "Hitresult.h"
#include "Selector.h"

#include "Entity/Entity.h"

#include <Graphics/Shader/Shader.h>
#include <Graphics/Camera/Camera.h>

#include <GLFW/glfw3.h>

using namespace MC;
using namespace Graphics;
using namespace Math;
using namespace Physics;

class Level;

class Player : public Entity
{
private:
	std::unique_ptr<Selector> m_Sel;
	bool m_MouseLeft;
	bool m_MouseRight;
	bool m_RayState;
	Hitresult m_HitResult;
public:
	Camera Cam;
public:
	Player(Level& level);
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
	void UpdateRayCast();
	void RenderPick(float time, Shader* shader);
	void Pick();
public:
	inline bool GetGround() { return attr.isGround; }
};
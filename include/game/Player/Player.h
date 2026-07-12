#pragma once
#include "Hitresult.h"
#include "Selector.h"

#include "Block/Block.h"
#include "Entity/Entity.h"

#include <Graphics/Shader/ShaderManager.h>
#include <Graphics/Camera/PerspectiveCamera.h>

#include <GLFW/glfw3.h>

using namespace MC;
using namespace Graphics;
using namespace Math;
using namespace Physics;

class Level;

class Player : public Entity
{
private:
	BlockType m_SelectedBlock;
	std::unique_ptr<Selector> m_Sel;
	Shader *m_Shader;
	bool m_MouseLeft;
	bool m_MouseRight;
	bool m_RayState;
	Hitresult m_HitResult;
public:
	PerspectiveCamera Cam;
public:
	Player(Level& level);
	~Player();
public:
	void Render(float alpha, float seconds) override;
	void Update() override;
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
	void RenderPick(float time);
	void Pick();
public:
	inline bool GetGround() { return attr.isGround; }
	inline BlockType GetSelectedBlock() { return m_SelectedBlock; }
public:
	inline void SetSelectedBlock(BlockType selectedBlock) { m_SelectedBlock = selectedBlock; };
};
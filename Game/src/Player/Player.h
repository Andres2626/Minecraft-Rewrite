#pragma once
#include <Graphics/Shader/Shader.h>
#include <Graphics/Camera/Camera.h>

#include <GLFW\glfw3.h>

class Level;

class Player {
protected:
	Level* lev;
private:
	bool m_Ground;
	glm::vec2 m_Size;
public:
	MC::Graphics::Camera cam;
	MC::Physics::AABB aabb;
	glm::vec3 pos;
	glm::vec3 movdelta;
public:
	Player(Level* level);
	~Player();
public:
	void Render(MC::Graphics::Shader* shader);
	void Update();
	void tick();
public:
	void Move(const glm::vec3& pos);
	void MoveRelative(glm::vec2 a, float speed);
	void turn(glm::vec2 mouse);
public:
	void ResetPos();
	void SetPos(const glm::vec3& newPos);
public:
	inline bool GetGround() { return this->m_Ground; }
};
#include "Player.h"

#include "Level/Level.h"
#include <App/Input.h>

using namespace MC;
using namespace App;

bool first = true;

static const double pi = 3.14159265358979323846;

Player::Player(Level* level)
	: m_Ground(false), m_Size({ 0.3f, 0.9f }), lev(level), cam(glm::vec3(0.0f))
{
	/* create camera */
	this->cam.fov = 70.0f;
	this->cam.near = 0.05f;
	this->cam.far = 100.0f;
	this->cam.Update();

	ResetPos();
}

Player::~Player() 
{

}

void Player::Render(MC::Graphics::Shader* shader) 
{
	glm::mat4 clip = this->cam.GetProj() * this->cam.GetView();
	shader->Set4x4("s_VP", clip);
}
	
void Player::Update() 
{
	this->cam.pos = this->pos;
	this->cam.Update();
}

void Player::Move(const glm::vec3& pos) 
{
	glm::vec3 org = pos;
	glm::vec3 a = pos;
	std::vector<MC::Physics::AABB> aabbs = this->lev->GetCubes(this->aabb.Expand(pos));

	for (MC::Physics::AABB& aabb : aabbs) {
		a.x = aabb.ClipXCollide(this->aabb, a.x);
		a.y = aabb.ClipYCollide(this->aabb, a.y);
		a.z = aabb.ClipZCollide(this->aabb, a.z);
	}

	this->aabb.Move({ a.x, a.y, a.z });
	this->m_Ground = org.y != a.y && org.y < 0.0f;

	if (org.x != a.x)
		this->movdelta.x = 0.0f;

	if (org.y != a.y)
		this->movdelta.y = 0.0f;

	if (org.z != a.z)
		this->movdelta.z = 0.0f;

	this->pos.x = (this->aabb.p0.x + this->aabb.p1.x) / 2.0f;
	this->pos.y = this->aabb.p0.y + 1.62f;
	this->pos.z = (this->aabb.p0.z + this->aabb.p1.z) / 2.0f;
}

void Player::MoveRelative(glm::vec2 a, float speed) 
{
	float dis = glm::length(a);
	float s, c;
	if (dis >= 0.01f) {
		dis = speed / sqrt(dis);
		a *= dis;
		s = (float)sin(this->cam.rot.x * pi / 180);
		c = (float)cos(this->cam.rot.x * pi / 180);
		this->movdelta.x += a.x * c - a.y * s;
		this->movdelta.z += a.y * c + a.x * s;
	}
}

void Player::turn(glm::vec2 pos) 
{
	pos *= 0.2f;
	this->cam.rot += pos;
	if (this->cam.rot.y > 89.0f)
		this->cam.rot.y = 89.0f;
	if (this->cam.rot.y < -89.0f)
		this->cam.rot.y = -89.0f;
}

void Player::tick() 
{
	glm::vec2 a(0.0f, 0.0f);

	/* TODO: Make the window independent of the player */
	if (Input::IsKeyPressed(GLFW_KEY_R))
		this->ResetPos();
	if (Input::IsKeyPressed(GLFW_KEY_W))
		a.x++;
	if (Input::IsKeyPressed(GLFW_KEY_S))
		a.x--;
	if (Input::IsKeyPressed(GLFW_KEY_A))
		a.y--;
	if (Input::IsKeyPressed(GLFW_KEY_D))
		a.y++;
	if (Input::IsKeyPressed(GLFW_KEY_SPACE) && this->m_Ground)
		this->movdelta.y = 0.12f;

	float hspeed = this->m_Ground ? 0.02f : 0.005f;
	MoveRelative(a, hspeed);
	this->movdelta.y -= 0.005f;
	Move(this->movdelta);
	this->movdelta.x *= 0.91f;
	this->movdelta.y *= 0.98f;
	this->movdelta.z *= 0.91f;
	if (this->m_Ground) {
		this->movdelta.x *= 0.8f;
		this->movdelta.z *= 0.8f;
	}
}

void Player::ResetPos() 
{
	glm::vec3 newpos;
	newpos.x = (float)(rand() % this->lev->size.x + 1);
	newpos.y = (float)(this->lev->size.z + 10);
	newpos.z = (float)(rand() % this->lev->size.y + 1);
	SetPos(newpos);
}

void Player::SetPos(const glm::vec3& newPos) 
{
	this->pos = newPos;
	this->cam.pos = newPos;
	this->aabb = MC::Physics::AABB({ newPos.x - this->m_Size.x, newPos.y - this->m_Size.y, newPos.z - this->m_Size.x },
		                           { newPos.x + this->m_Size.x, newPos.y + this->m_Size.y, newPos.z + this->m_Size.x });
	cam.Update();
}
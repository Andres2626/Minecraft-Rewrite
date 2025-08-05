#include "Player.h"

#include "Level/Level.h"
#include <App/Input.h>

using namespace MC;
using namespace App;

bool first = true;

static const double PI = 3.14159265358979323846;

Player::Player(Level* level)
	: m_Ground(false), m_Size({ 0.3f, 0.9f }), m_Lev(level), Cam(vec3(0.0f))
{
	/* create camera */
	Cam.fov = 70.0f;
	Cam.near = 0.05f;
	Cam.far = 100.0f;
	Cam.Update();

	ResetPos();
}

Player::~Player() 
{

}

void Player::Render() 
{
	Cam.pos = m_Pos;
	Cam.Update();
}

void Player::Move(const vec3& pos) 
{
	vec3 org = pos;
	vec3 a = pos;
	std::vector<AABB> aabbs = m_Lev->GetCubes(m_Box.Expand(pos));

	for (AABB& aabb : aabbs) {
		a.x = aabb.ClipXCollide(m_Box, a.x);
		a.y = aabb.ClipYCollide(m_Box, a.y);
		a.z = aabb.ClipZCollide(m_Box, a.z);
	}

	m_Box.Move({ a.x, a.y, a.z });
	this->m_Ground = (org.y != a.y) && (org.y < 0.0f);

	if (org.x != a.x)
		m_DMov.x = 0.0f;

	if (org.y != a.y)
		m_DMov.y = 0.0f;

	if (org.z != a.z)
		m_DMov.z = 0.0f;

	m_Pos.x = (m_Box.p0.x + m_Box.p1.x) / 2.0f;
	m_Pos.y = m_Box.p0.y + 1.62f;
	m_Pos.z = (m_Box.p0.z + m_Box.p1.z) / 2.0f;
}

void Player::MoveRelative(vec2 a, float speed) 
{
	float dis = length(a);
	float s = sin(radians(Cam.rot.x));
	float c = cos(radians(Cam.rot.x));
	if (dis >= 0.01f) {
		dis = speed / sqrt(dis);
		a *= dis;
		m_DMov.x += a.x * c - a.y * s;
		m_DMov.z += a.y * c + a.x * s;
	}
}

void Player::MouseMove(vec2 pos) 
{
	/* set mouse sensibility */
	pos *= 0.2f;
	Cam.rot += pos;

	/* Block camera rotation */
	if (Cam.rot.y > 89.0f)
		Cam.rot.y = 89.0f;
	if (Cam.rot.y < -89.0f)
		Cam.rot.y = -89.0f;
}

void Player::Update() 
{
	vec2 a(0.0f, 0.0f);
	float hspeed;

	/* DONE: Window independent of the Player */
	if (Input::IsKeyPressed(MC_KEY_R))
		this->ResetPos();
	if (Input::IsKeyPressed(MC_KEY_W))
		a.x++;
	if (Input::IsKeyPressed(MC_KEY_S))
		a.x--;
	if (Input::IsKeyPressed(MC_KEY_A))
		a.y--;
	if (Input::IsKeyPressed(MC_KEY_D))
		a.y++;
	if (Input::IsKeyPressed(MC_KEY_SPACE) && this->m_Ground)
		m_DMov.y = 0.12f;

	/* calculate player horizontal speed */
	hspeed = this->m_Ground ? 0.02f : 0.005f;
	MoveRelative(a, hspeed);

	m_DMov.y -= 0.005f; /* gravity speed */

	Move(m_DMov);

	m_DMov.x *= 0.91f;
	m_DMov.y *= 0.98f;
	m_DMov.z *= 0.91f;

	if (this->m_Ground) {
		m_DMov.x *= 0.8f;
		m_DMov.z *= 0.8f;
	}
}

void Player::ResetPos() 
{
	vec3 newPos;
	ivec3 Size = m_Lev->GetSize();

	/* Calculate new player position */
	newPos.x = (float)(rand() % Size.x + 1);
	newPos.y = (float)(Size.z + 10);
	newPos.z = (float)(rand() % Size.y + 1);

	/* Restore player position */
	SetPos(newPos);
}

void Player::SetPos(const vec3& newPos) 
{
	/* Set player vectors */
	m_Pos = newPos;
	Cam.pos = newPos;

	/* Calculate new AABB */
	m_Box = AABB({ newPos.x - this->m_Size.x, newPos.y - this->m_Size.y, newPos.z - this->m_Size.x },
		         { newPos.x + this->m_Size.x, newPos.y + this->m_Size.y, newPos.z + this->m_Size.x });
	Cam.Update();
}
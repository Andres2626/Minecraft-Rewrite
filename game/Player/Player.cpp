#include "Player/Player.h"

#include "GameProperties.h"
#include "Level/Level.h"

#include <App/Input.h>
#include <App/Application.h>
#include <Graphics/Camera/Ray.h>
#include <Utils/Util.h>

using namespace MC;
using namespace App;

Player::Player(Level &level)
	: Entity(level), Cam({ 0.0f, 0.0f, 0.0f }), m_SelectedBlock(BlockType::ROCK)
{
	float x = (float)App::Application::GetProperties().x;
	float y = (float)App::Application::GetProperties().y;

	Cam.aspect = x / y;
	Cam.fov = 70.0f;
	Cam.near = 0.05f;
	Cam.far = 1000.0f;
	Cam.Update();

	/* create selector renderer */
	m_Sel = std::make_unique<Selector>();
	attr.heightOffset = 1.62f;
	m_Shader = &ShaderManager::Get("selector");
	ResetPos();
}

Player::~Player() 
{

}

void Player::Render(float alpha, float seconds)
{
	vec3 p1 = mix(attr.oldPos, attr.pos, alpha);
	Cam.pos = p1;
	Cam.Update();
}

void Player::Move(const vec3 &pos) 
{
	Entity::Move(pos);
}

void Player::MoveRelative(vec2 a, float speed) 
{
	Entity::MoveRelative(a, speed);
}

void Player::MouseMove(vec2 pos) 
{
	/* set mouse sensibility */
	pos *= 0.2f;
	Cam.rot += vec2(pos.x, pos.y);
	attr.rot += vec2(pos.x, pos.y);

	/* Block camera rotation */
	Cam.rot.y= std::clamp(Cam.rot.y, -89.0f, 89.0f);
}

bool Player::Raycast(const vec3 &org, const vec3 &dir, Hitresult &ret)
{
	vec3 d = dir;
	vec3 lpoint = org;
	ivec3 lblock = floor(org);
	Ray camray;

	for (float t = 0.0f; t < 5.0f; t += 0.1f) {
		/* get point by parametric equation f(t) = dir * t + pos */
		vec3 equation = camray.GetRay(t, org, d);

		/* impact block */
		ivec3 blockpos = floor(equation);

		if (blockpos != lblock && m_Level.IsSolidTile(blockpos)) {
			ivec3 normal = lblock - blockpos;

			/* obtain block face normal */
			if (normal == ivec3(0)) {
				vec3 ad = abs(d);

				if (ad.x >= ad.y && ad.x >= ad.z)
					normal = ivec3((dir.x > 0) ? -1 : 1, 0, 0);
				else if (ad.y >= ad.x && ad.y >= ad.z)
					normal = ivec3(0, (dir.y > 0) ? -1 : 1, 0);
				else
					normal = ivec3(0, 0, (dir.z > 0) ? -1 : 1);
			}

			ret.block = blockpos;
			ret.dis = t;
			ret.face = normal;
			ret.point = equation;

			return true;
		}

		lpoint = equation;
		lblock = blockpos;
	}

	return false;
}

void Player::Update() 
{
	attr.oldPos = attr.pos;
	vec2 a(0.0f, 0.0f);
	float hspeed;

	if (Input::IsKeyPressed(MC_KEY_R))
		ResetPos();
	if (Input::IsKeyPressed(MC_KEY_W))
		a.x++;
	if (Input::IsKeyPressed(MC_KEY_S))
		a.x--;
	if (Input::IsKeyPressed(MC_KEY_A))
		a.y--;
	if (Input::IsKeyPressed(MC_KEY_D))
		a.y++;
	if (Input::IsKeyPressed(MC_KEY_SPACE) && attr.isGround)
		attr.motion.y = 0.5f;

	/* calculate player horizontal speed */
	hspeed = attr.isGround ? 0.1f : 0.02f;
	MoveRelative(a, hspeed);

	attr.motion.y -= 0.08f; /* gravity speed */

	Move(attr.motion);

	attr.motion.x *= 0.91f;
	attr.motion.y *= 0.98f;
	attr.motion.z *= 0.91f;

	if (attr.isGround) {
		attr.motion.x *= 0.7f;
		attr.motion.z *= 0.7f;
	}
}

void Player::ResetPos() 
{
	vec3 newPos;
	ivec3 Size = m_Level.GetSize();

	/* Calculate new player position */
	newPos.x = (float)(rand() % Size.x + 1);
	newPos.y = (float)(Size.y + 10);
	newPos.z = (float)(rand() % Size.z + 1);

	/* Restore player position */
	SetPos(newPos);
}

void Player::SetPos(const vec3 &newPos) 
{
	/* Set player vectors */
	attr.pos = newPos;
	Cam.pos = newPos;

	/* Calculate new AABB */
	attr.box = AABB({ newPos.x - attr.size.x, newPos.y - attr.size.y, newPos.z - attr.size.x },
		            { newPos.x + attr.size.x, newPos.y + attr.size.y, newPos.z + attr.size.x });

	Cam.Update();
}

void Player::UpdateRayCast()
{
	vec3 ray = Cam.front;
	vec3 org = Cam.pos;
	m_RayState = Raycast(org, ray, m_HitResult);
}

void Player::RenderPick(float time)
{
	m_Shader->Bind();
	m_Shader->Set4x4("s_VP", Cam.GetProjection() * Cam.GetView());

	if (!m_RayState)
		return;

	/* Render player selector */
	m_Sel->SetHit(m_HitResult);
	m_Sel->Render(Cam, m_Shader, time);
}

void Player::Pick()
{
	if (!m_RayState)
		return;

	/* Get mouse button status */
	bool left = Input::IsMouseButtonPressed(MC_MOUSE_BUTTON_1);
	bool right = Input::IsMouseButtonPressed(MC_MOUSE_BUTTON_2);

	/* Avoid click spam */
	if (left && !m_MouseLeft)
		m_Level.SetTile(m_HitResult.block + m_HitResult.face, m_SelectedBlock); /* set block */

	if (right && !m_MouseRight)
		m_Level.SetTile(m_HitResult.block, BlockType::AIR); /* delete block */
		
	/* Update mouse state */
	m_MouseLeft = left;
	m_MouseRight = right;
}

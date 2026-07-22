#include "Particle/Particle.h"

static float NormRand()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

Particle::Particle(Level &level, const vec3 &pos, const vec3 &motion, u32t tex)
	: Entity(level)
{
	m_Tex = tex;
	SetSize({ 0.2f, 0.2f });
	attr.heightOffset = attr.size.y / 2.0f;

	SetPos(pos);
	attr.motion = motion + vec3((NormRand() * 2.0f - 1.0f) * 0.4f);
	attr.speed = (NormRand() + NormRand() + 1.0f) * 0.15f;

	float distance = sqrt(attr.motion.x * attr.motion.x + attr.motion.y * attr.motion.y + attr.motion.z * attr.motion.z);
	attr.motion.x = attr.motion.x / distance * attr.speed * 0.7f;
	attr.motion.y = attr.motion.y / distance * attr.speed;
	attr.motion.z = attr.motion.z / distance * attr.speed * 0.7f;
	m_TextureOff = vec2(NormRand() * 3.0f, NormRand() * 3.0f);
}

Particle::~Particle()
{
}

void Particle::Update()
{
	attr.oldPos = attr.pos;

	if (NormRand() < 0.1f)
		attr.isDead = true;

	attr.motion.y -= 0.06f;

	Move(attr.motion);
	attr.motion *= 0.98f;

	if (attr.isGround) {
		attr.motion.x *= 0.7f;
		attr.motion.z *= 0.7f;
	}
}

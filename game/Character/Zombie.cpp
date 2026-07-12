#include "Character/Zombie.h"

#include "Log/Log.h"

#include <chrono>

static float NormRand()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

Zombie::Zombie(Level& level, vec3 pos)
    : Entity(level), scalefac(0.058333334f)
{
    SetPos(pos);
    attr.heightOffset = 0.0f;
    timeoff = NormRand();
    rotA = (NormRand() + 1.0f) * 0.01f;
    rot = NormRand() * (float)M_PI * 2.0f;
    speed = 1.0f;
    m_Shader = &ShaderManager::Get("character");

    for (auto& r : m_PartRot) r = vec3(0.0f);
}

Zombie::~Zombie()
{
}

void Zombie::Update()
{
    attr.oldPos = attr.pos;
    vec2 a = vec2(0.0f, 0.0f);
    rot += rotA;
    rotA = rotA * 0.99f;
    rotA = rotA + (NormRand() - NormRand()) * NormRand() * NormRand() * 0.009999999776482582f;
    a.x = sin(rot);
    a.y = cos(rot);
    if (attr.isGround && NormRand() < 0.01f)
        attr.motion.y = 0.5f;

    MoveRelative(a, attr.isGround ? 0.1f : 0.02f);
    attr.motion.y -= 0.08f;
    Move(attr.motion);

    attr.motion.x *= 0.91f;
    attr.motion.y *= 0.98f;
    attr.motion.z *= 0.91f;

    if (attr.pos.y > 100.0f)
        ResetPos();

    if (attr.isGround) {
        attr.motion.x *= 0.7f;
        attr.motion.z *= 0.7f;
    }

    if (attr.pos.y < -30.0f)
        attr.isDead = true;
}

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
    timeoff = NormRand() * 1239813.0f;
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

void Zombie::Render(float alpha, float seconds)
{
    mat4 model(1.0f);
    float t = seconds * 10.0f * speed + timeoff;
    float dy = -fabsf((float)sin(0.6662 * t)) * 5.0f - 23.0f;

    model = translate(model, mix(attr.oldPos, attr.pos, alpha));
    model = scale(model, { 1.0f, -1.0f, 1.0f });
    model = scale(model, { scalefac, scalefac, scalefac });
    model = translate(model, { 0.0f, dy, 0.0f });
    model = rotate(model, rot + 180.0f, { 0.0f, 1.0f, 0.0f });

    m_PartRot[0] = { (float)(0.8f * sin(t)), (float)(sin(0.83f * t)), 0.0f };
    m_PartRot[1] = { 0.0f, 0.0f, 0.0f };
    m_PartRot[2] = { (float)(sin(0.6662f * t + M_PI)) * 2.0f, 0.0f, (float)(sin(0.2312f * t) + 1.0f) };
    m_PartRot[3] = { (float)(sin(0.6662f * t)) * 2.0f, 0.0f, (float)(sin(0.2812f * t) - 1.0f) };
    m_PartRot[4] = { (float)(sin(0.6662f * t)) * 1.4f, 0.0f, 0.0f };
    m_PartRot[5] = { (float)(sin(0.6662f * t + M_PI)) * 1.4f, 0.0f, 0.0f };

    const auto &parts = ZombieModel::Get().GetParts();

    for (size_t i = 0; i < parts.size(); i++) {
        mat4 local(1.0f);
        local = translate(local, parts[i].rot);
        local = rotate(local, m_PartRot[i].z, { 0.0f, 0.0f, 1.0f });
        local = rotate(local, m_PartRot[i].y, { 0.0f, 1.0f, 0.0f });
        local = rotate(local, m_PartRot[i].x, { 1.0f, 0.0f, 0.0f });

        m_Shader->Set4x4("s_M", model * local);
        Renderer::DrawElements(GL_TRIANGLES, parts[i].size, (void*)(parts[i].offset * sizeof(u32t)));
    }
}

#include "Character/Zombie.h"

#include "Log/Log.h"

#include <chrono>

static float NormRand()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

Zombie::Zombie(Level& level, vec3 pos)
    : Entity(level), scalefac(0.058333334f), partOffset(0)
{
    SetPos(pos);
    attr.heightOffset = 0.0f;
    timeoff = (float)NormRand() * 1239813.0f;
    rotA = (float)(NormRand() + 1.0f) * 0.01f;
    rot = (float)NormRand() * M_PI * 2.0f;
    speed = 1.0f;
    VAO = std::make_unique<VertexArray>();
    VBO = std::make_unique<VertexBuffer>();
    IBO = std::make_unique<IndexBuffer>();
    Build();
}

Zombie::~Zombie()
{

}

void Zombie::Build()
{
    Cube head({ 0.0f, 0.0f });
    Cube body({ 16.0f, 16.0f });
    Cube arm0({ 40.0f, 16.0f });
    Cube arm1({ 40.0f, 16.0f });
    Cube leg0({ 0.0f, 16.0f });
    Cube leg1({ 0.0f, 16.0f });

    head.AddBox({ -4.0f, -8.0f, -4.0f }, { 8.0f, 8.0f, 8.0f }, vertices, indices);
    body.AddBox({ -4.0f, 0.0f, -2.0f }, { 8.0f, 12.0f, 4.0f }, vertices, indices);
    arm0.AddBox({ -3.0f, -2.0f, -2.0f }, { 4.0f, 12.0f, 4.0f }, vertices, indices);
    arm1.AddBox({ -1.0f, -2.0f, -2.0f }, { 4.0f, 12.0f, 4.0f }, vertices, indices);
    leg0.AddBox({ -2.0f, 0.0f, -2.0f }, { 4.0f, 12.0f, 4.0f }, vertices, indices);
    leg1.AddBox({ -2.0f, 0.0f, -2.0f }, { 4.0f, 12.0f, 4.0f }, vertices, indices);
    arm0.pos = vec3(-5.0f, -2.0f, 0.0f);
    arm1.pos = vec3(5.0f, -2.0f, 0.0f);
    leg0.pos = vec3(-2.0f, 12.0f, 0.0f);
    leg1.pos = vec3(2.0f, 12.0f, 0.0f);

    /* build renderer */
    PushPart(head);
    PushPart(body);
    PushPart(arm0);
    PushPart(arm1);
    PushPart(leg0);
    PushPart(leg1);

    VertexLayout VL;
    VAO->Bind();
    VBO->Build(vertices.size() * sizeof(float), vertices.data());
    IBO->Build(indices.size(), indices.data());
    VL.AddAttribute(SHADER_VERTEX_BIT, 5 * sizeof(float), (void*)0);
    VL.AddAttribute(SHADER_TEX_BIT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO->Link(VL);
    VAO->Unbind();
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

void Zombie::Render(Shader *shader, float alpha, float seconds)
{ 
    mat4 model(1.0f);
    float t = seconds * 10.0f * speed + timeoff;
    float dy = -fabsf((float)sin(0.6662 * t)) * 5.0f - 23.0f;
    model = translate(model, mix(attr.oldPos, attr.pos, alpha));
    model = scale(model, { 1.0f, -1.0f, 1.0f });
    model = scale(model, { scalefac, scalefac, scalefac });
    model = translate(model, { 0.0f, dy, 0.0f });
    model = rotate(model, rot + 180.0F, { 0.0f, 1.0f, 0.0f });
    parts[0].cube.rot.y = (float)(sin(0.83f * t)) * 1.0f;
    parts[0].cube.rot.x = (float)(0.8f * sin(t));
    parts[2].cube.rot.x = (float)(sin(0.6662f * t + M_PI)) * 2.0f;
    parts[2].cube.rot.z = (float)(sin(0.2312f * t) + 1.0f) * 1.0f;
    parts[3].cube.rot.x = (float)(sin(0.6662f * t)) * 2.0f;
    parts[3].cube.rot.z = (float)(sin(0.2812f * t) - 1.0f) * 1.0f;
    parts[4].cube.rot.x = (float)(sin(0.6662f * t)) * 1.4f;
    parts[5].cube.rot.x = (float)(sin(0.6662f * t + M_PI)) * 1.4f;

    VAO->Bind();

    for (auto& a : parts) {
        shader->Set4x4("s_M", model * a.cube.GetModelMatrix());
        Renderer::DrawElements(GL_TRIANGLES, a.size, (void*)(a.offset * sizeof(u32t)));
    }

    VAO->Unbind();
}

void Zombie::PushPart(const Cube& cube)
{
    parts.push_back({ partOffset, 36, cube });
    partOffset += 36;
}
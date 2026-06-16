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
    attr.pos = pos;
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

    MC::Graphics::VertexLayout VL;
    VBO->Build(vertices.size() * sizeof(float), vertices.data());
    IBO->Build(indices.size(), indices.data());
    VL.AddAttribute(SHADER_VERTEX_BIT, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    VL.AddAttribute(SHADER_TEX_BIT, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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
        attr.delta.y = 0.12f;

    MoveRelative(a, attr.isGround ? 0.02f : 0.005f);
    attr.delta.y = attr.delta.y - 0.005f;
    Move(attr.delta);
    attr.delta.x *= 0.91f;
    attr.delta.y *= 0.98f;
    attr.delta.z *= 0.91f;

    if (attr.pos.y > 100.0f)
        ResetPos();

    if (attr.isGround) {
        attr.delta.x *= 0.8f;
        attr.delta.z *= 0.8f;
    }
}

void Zombie::Render(float alpha, Shader* shader, float seconds)
{
    VAO->Bind();
    mat4 model(1.0f);
    float time = seconds * 10.0f * speed + timeoff;
    float yy = -fabsf((float)sin(time * 0.6662)) * 5.0f - 23.0f;
    model = translate(model, mix(attr.oldPos, attr.pos, alpha));
    model = scale(model, { 1.0f, -1.0f, 1.0f });
    model = scale(model, { scalefac, scalefac, scalefac });
    model = translate(model, { 0.0f, yy, 0.0f });
    model = rotate(model, rot + 180.0F, { 0.0f, 1.0f, 0.0f });
    parts[0].cube.rot.y = (float)(sin(time * 0.83f)) * 1.0f;
    parts[0].cube.rot.x = (float)(sin(time) * 0.8f);
    parts[2].cube.rot.x = (float)(sin(time * 0.6662f + M_PI)) * 2.0f;
    parts[2].cube.rot.z = (float)(sin(time * 0.2312f) + 1.0f) * 1.0f;
    parts[3].cube.rot.x = (float)(sin(time * 0.6662f)) * 2.0f;
    parts[3].cube.rot.z = (float)(sin(time * 0.2812f) - 1.0f) * 1.0f;
    parts[4].cube.rot.x = (float)(sin(time * 0.6662f)) * 1.4f;
    parts[5].cube.rot.x = (float)(sin(time * 0.6662f + M_PI)) * 1.4f;

    for (auto& a : parts) {
        shader->Set4x4("s_M", model * a.cube.GetModelMatrix());
        Renderer::DrawElements(GL_TRIANGLES, a.size, (void*)(a.offset * sizeof(unsigned int)));
    }

    VAO->Unbind();
}

void Zombie::PushPart(const Cube& cube)
{
    parts.push_back({ partOffset, 36, cube });
    partOffset += 36;
}
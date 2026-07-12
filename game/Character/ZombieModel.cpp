#include "Character/ZombieModel.h"

ZombieModel::ZombieModel()
    : m_partOffset(0)
{
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(GL_STATIC_DRAW);
    m_IBO = std::make_unique<IndexBuffer>();
    m_InstanceBuffer = std::make_unique<VertexBuffer>(GL_DYNAMIC_DRAW);
    Build();
}

ZombieModel::~ZombieModel()
{
}

ZombieModel& ZombieModel::Get()
{
    static ZombieModel instance;
    return instance;
}

void ZombieModel::Build()
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

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

    VertexLayout VL, IVL;
    m_VAO->Bind();
    m_VBO->Build(vertices.size() * sizeof(float), vertices.data());
    m_IBO->Build(indices.size(), indices.data());
    VL.AddAttribute(SHADER_VERTEX_BIT, 5 * sizeof(float), (void*)0);
    VL.AddAttribute(SHADER_TEX_BIT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    m_VAO->Link(VL);

    m_InstanceBuffer->Resize(MAX_INSTANCES * sizeof(ZombieInstance));
    IVL.AddAttribute(5, sizeof(ZombieInstance), (void*)offsetof(ZombieInstance, pos), 3);
    IVL.AddAttribute(6, sizeof(ZombieInstance), (void*)offsetof(ZombieInstance, rotY), 1);
    IVL.AddAttribute(7, sizeof(ZombieInstance), (void*)offsetof(ZombieInstance, timeoff), 1);
    IVL.AddAttribute(8, sizeof(ZombieInstance), (void*)offsetof(ZombieInstance, speed), 1);
    IVL.AddAttribute(9, sizeof(ZombieInstance), (void*)offsetof(ZombieInstance, brig), 1);

    m_VAO->Unbind();
}

void ZombieModel::PushPart(const Cube& cube)
{
    m_Parts.push_back({ m_partOffset, 36, cube.pos });
    m_partOffset += 36;
}

void ZombieModel::Bind() const
{
    m_VAO->Bind();
}

void ZombieModel::Unbind() const
{
    m_VAO->Unbind();
}

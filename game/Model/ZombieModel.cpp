#include "Model/ZombieModel.h"

ZombieModel::ZombieModel()
    : m_partOffset(0)
{
    VertexLayout VL, IVL;
    VL.AddAttribute<Math::vec3>(SHADER_VERTEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    VL.AddAttribute<Math::vec2>(SHADER_TEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    IVL.AddAttribute<vec3>(5, sizeof(ZombieInstance), (void*)offsetof(ZombieInstance, pos), 1);
    IVL.AddAttribute<float>(6, sizeof(ZombieInstance), (void*)offsetof(ZombieInstance, rotY), 1);
    IVL.AddAttribute<float>(7, sizeof(ZombieInstance), (void*)offsetof(ZombieInstance, timeoff), 1);
    IVL.AddAttribute<float>(8, sizeof(ZombieInstance), (void*)offsetof(ZombieInstance, speed), 1);
    IVL.AddAttribute<float>(9, sizeof(ZombieInstance), (void*)offsetof(ZombieInstance, brig), 1);

    m_Mesh = std::make_unique<Mesh>(VL);
    m_InsMesh = std::make_unique<InstancedMesh>(*m_Mesh, IVL, sizeof(ZombieInstance), 1000);
    Build();
}

ZombieModel::~ZombieModel()
{

}

void ZombieModel::Build()
{
    MeshData buff;

    Cube head({ 0.0f, 0.0f });
    Cube body({ 16.0f, 16.0f });
    Cube arm0({ 40.0f, 16.0f });
    Cube arm1({ 40.0f, 16.0f });
    Cube leg0({ 0.0f, 16.0f });
    Cube leg1({ 0.0f, 16.0f });

    head.AddBox({ -4.0f, -8.0f, -4.0f }, { 8.0f, 8.0f, 8.0f }, buff);
    body.AddBox({ -4.0f, 0.0f, -2.0f }, { 8.0f, 12.0f, 4.0f }, buff);
    arm0.AddBox({ -3.0f, -2.0f, -2.0f }, { 4.0f, 12.0f, 4.0f }, buff);
    arm1.AddBox({ -1.0f, -2.0f, -2.0f }, { 4.0f, 12.0f, 4.0f }, buff);
    leg0.AddBox({ -2.0f, 0.0f, -2.0f }, { 4.0f, 12.0f, 4.0f }, buff);
    leg1.AddBox({ -2.0f, 0.0f, -2.0f }, { 4.0f, 12.0f, 4.0f }, buff);
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

    m_IndexSize = (u32t)buff.indices.size();

    m_Mesh->Bind();
    m_Mesh->Build(&buff);
    m_InsMesh->Bind();
    m_InsMesh->Build();
    m_InsMesh->Unbind();
    m_Mesh->Unbind();
}

void ZombieModel::PushPart(const Cube& cube)
{
    m_Parts.push_back({ m_partOffset, 36, cube.pos });
    m_partOffset += 36;
}

void ZombieModel::Bind() const
{
    m_Mesh->Bind();
}

void ZombieModel::Unbind() const
{
    m_Mesh->Unbind();
}

u32t ZombieModel::GetIndexCount() const
{
    return m_IndexSize;
}

InstancedMesh& ZombieModel::GetInstancedMesh() const
{
    return *m_InsMesh;
}

std::vector<ZombiePart> &ZombieModel::GetParts()
{ 
    return m_Parts; 
};

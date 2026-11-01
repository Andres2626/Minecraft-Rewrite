#include "Model/ParticleModel.h"

ParticleModel::ParticleModel()
{
	VertexLayout VL, IVL;
	VL.AddAttribute<Math::vec3>(SHADER_VERTEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	IVL.AddAttribute<vec3>(5, sizeof(ParticleInstance), (void*)offsetof(ParticleInstance, pos), 1);
	IVL.AddAttribute<u32t>(6, sizeof(ParticleInstance), (void*)offsetof(ParticleInstance, tex), 1);
	IVL.AddAttribute<vec2>(7, sizeof(ParticleInstance), (void*)offsetof(ParticleInstance, texOffset), 1);

	m_Mesh = std::make_unique<Mesh>(VL);
	m_InsMesh = std::make_unique<InstancedMesh>(*m_Mesh, IVL, sizeof(ParticleInstance), 1000);
	Build();
}

ParticleModel::~ParticleModel()
{

}

void ParticleModel::Build()
{
	MeshData buff;
	Vertex v0({ -0.5f, -0.5f, 0.0f });
	Vertex v1({ 0.5f, -0.5f, 0.0f });
	Vertex v2({ 0.5f,  0.5f, 0.0f });
	Vertex v3({ -0.5f,  0.5f, 0.0f });
	MeshFactory::AddQuad(buff, v0, v1, v2, v3);
	MeshFactory::AddIndices(buff, 0);

	m_IndexSize = (u32t)buff.indices.size();

	m_Mesh->Bind();
	m_Mesh->Build(&buff);
	m_InsMesh->Bind();
	m_InsMesh->Build();
	m_InsMesh->Unbind();
	m_Mesh->Unbind();	
}

void ParticleModel::Bind() const
{
	m_Mesh->Bind();
}

void ParticleModel::Unbind() const
{
	m_Mesh->Unbind();
}

u32t ParticleModel::GetIndexCount() const
{
	return m_IndexSize;
}

InstancedMesh &ParticleModel::GetInstancedMesh() const
{
	return *m_InsMesh;
}

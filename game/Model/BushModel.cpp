#include "Model/BushModel.h"

#include "Block/Block.h"
#include "Block/BlockManager.h"
#include "Renderer/StaticModelRenderer.h"

#include <Graphics/MeshFactory.h>

#define _USE_MATH_DEFINES
#include <math.h>

BushModel::BushModel()
{
	VertexLayout VL, IVL;
	VL.AddAttribute<Math::vec3>(SHADER_VERTEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	VL.AddAttribute<Math::vec3>(SHADER_COLOR_BIT, sizeof(Vertex), (void*)offsetof(Vertex, color));
	VL.AddAttribute<Math::vec2>(SHADER_TEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	IVL.AddAttribute<vec3>(5, sizeof(StaticBlockInstance), (void*)offsetof(StaticBlockInstance, pos), 1);
	IVL.AddAttribute<float>(6, sizeof(StaticBlockInstance), (void*)offsetof(StaticBlockInstance, brightness), 1);

	m_Mesh = std::make_unique<Mesh>(VL);
	m_InsMesh = std::make_unique<InstancedMesh>(*m_Mesh, IVL, sizeof(StaticBlockInstance), 1000);
	Build();
}

BushModel::~BushModel()
{

}

void BushModel::Build()
{
	MeshData buff;
	Block &block = BlockManager::GetBlockType(BlockType::BUSH);

	float minU = block.GetTexID() % 16 / 16.0F;
	float minV = minU + 999 / 16000.0F;
	float maxU = (float)(block.GetTexID() / 16) / 16.0F;
	float maxV = maxU + 999 / 16000.0F;

	for (int i = 0; i < 2; i++)
	{
		int count = (int)buff.vertices.size();

		float s = (float)sin(i * M_PI / 2 + M_PI / 4) / 2;
		float c = (float)cos(i * M_PI / 2 + M_PI / 4) / 2;

		float minX = 0.5F - s;
		float maxX = 0.5F + s;
		float minY = 0.0F;
		float maxY = 1.0F;
		float minZ = 0.5F - c;
		float maxZ = 0.5F + c;

		buff.vertices.push_back({ { minX, maxY, minZ }, { 0.0f, 0.0f, 0.0f }, { minV, maxU } });
		buff.vertices.push_back({ { maxX, maxY, maxZ }, { 0.0f, 0.0f, 0.0f }, { minU, maxU } });
		buff.vertices.push_back({ { maxX, minY, maxZ }, { 0.0f, 0.0f, 0.0f }, { minU, maxV } });
		buff.vertices.push_back({ { minX, minY, minZ }, { 0.0f, 0.0f, 0.0f }, { minV, maxV } });

		MeshFactory::AddIndices(buff, count);
	}

	m_IndexCount = (u32t)buff.indices.size();

	m_Mesh->Bind();
	m_Mesh->Build(&buff);
	m_InsMesh->Bind();
	m_InsMesh->Build();
	m_InsMesh->Unbind();
	m_Mesh->Unbind();
}

void BushModel::Bind() const
{
	m_Mesh->Bind();
}

void BushModel::Unbind() const
{
	m_Mesh->Unbind();
}

u32t BushModel::GetIndexCount() const
{
	return m_IndexCount;
}

InstancedMesh &BushModel::GetInstancedMesh() const
{
	return *m_InsMesh;
}

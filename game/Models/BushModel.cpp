#include "Model/BushModel.h"
#include "Renderer/StaticModelRenderer.h"

#include "Graphics/MeshFactory.h"

#include "Block/Block.h"
#include "Block/BlockManager.h"

BushModel::BushModel()
{
	m_Mesh = std::make_unique<Mesh>(&m_MeshData);
	m_InstanceBuffer = std::make_unique<VertexBuffer>(GL_DYNAMIC_DRAW);
	Build();
}

BushModel::~BushModel()
{
}

void BushModel::Build()
{
	Block& block = BlockManager::GetBlockType(BlockType::BUSH);

	float minU = block.GetTexID() % 16 / 16.0F;
	float minV = minU + 999 / 16000.0F;
	float maxU = (float)(block.GetTexID() / 16) / 16.0F;
	float maxV = maxU + 999 / 16000.0F;

	for (int i = 0; i < 2; i++)
	{
		int count = (int)m_MeshData.vertices.size();

		float s = (float)sin(i * M_PI / 2 + M_PI / 4) / 2;
		float c = (float)cos(i * M_PI / 2 + M_PI / 4) / 2;

		float minX = 0.5F - s;
		float maxX = 0.5F + s;
		float minY = 0.0F;
		float maxY = 1.0F;
		float minZ = 0.5F - c;
		float maxZ = 0.5F + c;

		m_MeshData.vertices.push_back({ { minX, maxY, minZ }, { 0.0f, 0.0f, 0.0f }, { minV, maxU } });
		m_MeshData.vertices.push_back({ { maxX, maxY, maxZ }, { 0.0f, 0.0f, 0.0f }, { minU, maxU } });
		m_MeshData.vertices.push_back({ { maxX, minY, maxZ }, { 0.0f, 0.0f, 0.0f }, { minU, maxV } });
		m_MeshData.vertices.push_back({ { minX, minY, minZ }, { 0.0f, 0.0f, 0.0f }, { minV, maxV } });

		MeshFactory::AddIndices(m_MeshData, count);
	}

	m_IndexCount = m_MeshData.indices.size();

	m_Mesh->Bind();
	m_Mesh->Build();

	VertexLayout IVL;
	m_InstanceBuffer->Resize(1000 * sizeof(StaticBlockInstance));
	IVL.AddAttribute<vec3>(5, sizeof(StaticBlockInstance), (void*)offsetof(StaticBlockInstance, pos), 1);
	IVL.AddAttribute<float>(6, sizeof(StaticBlockInstance), (void*)offsetof(StaticBlockInstance, brightness), 1);
	m_InstanceBuffer->SetVertexLayout(IVL);
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

VertexBuffer& BushModel::GetInstanceBuffer()
{
	return *m_InstanceBuffer;
}

u32t BushModel::GetIndexCount() const
{
	return m_IndexCount;
}

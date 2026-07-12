#include "Chunk/ChunkManager.h"

#include "Level/Level.h"


ChunkManager::ChunkManager(Level *lev)
	: m_Level(lev), m_ChunkUpdates(0), m_DrawCalls(true)
{
	m_Shader = &ShaderManager::Get("chunk");
}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::Create()
{
	/* avoid set model matrix every frame */
	m_Shader->Bind();
	m_Shader->Set4x4("s_M", mat4(1.0f));

	auto ab = m_Level->GetSize();
	for (int cx = 0; cx < m_Level->GetSize().x / CHUNK_XYZ; cx++) {
		for (int cy = 0; cy < m_Level->GetSize().y / CHUNK_XYZ; cy++) {
			for (int cz = 0; cz < m_Level->GetSize().z / CHUNK_XYZ; cz++) {
				int index = GetChunkIndex({ cx, cy, cz });

				/*
				 * for avoid call to destructor when chunk inserting in the
				 * map use this instead of map::emplace(index chunk{...})
				 */
				m_Chunks.emplace(std::piecewise_construct,
					std::forward_as_tuple(index),
					std::forward_as_tuple(m_Level, ivec3{ cx, cy, cz }));
			}
		}
	}
}

void ChunkManager::Render(Player *player)
{
	/* check if chunk is in camera frustum */
	for (auto& n : m_Chunks) {
		float ply = player->attr.pos.y / 16;
		bool isInFrustum = player->Cam.InFrustum(n.second.GetBox());
		if (isInFrustum) {
			n.second.Render();
		}
	}
}

void ChunkManager::Update()
{
	int builds = 0;

	while (!m_DirtyChunks.empty() && builds < 2)
	{
		int index = m_DirtyChunks.front();
		m_DirtyChunks.pop();

		auto it = m_Chunks.find(index);

		if (it != m_Chunks.end()) {
			it->second.Build();
			it->second.SetDirty(false);

			m_ChunkUpdates++;
		}

		builds++;
	}
}

void ChunkManager::Tick()
{
	m_ChunkUpdates = 0;
	m_DrawCalls = 0;
}

void ChunkManager::MarkDirty(const ivec3 &blockpos)
{
	ivec3 chunk = blockpos / CHUNK_XYZ;

	AddDirtyChunk(chunk);

	int lx = blockpos.x & (CHUNK_XYZ - 1);
	int ly = blockpos.y & (CHUNK_XYZ - 1);
	int lz = blockpos.z & (CHUNK_XYZ - 1);

	/* add adjacent chunks if block is a border  */
	if (lx == 0)
		AddDirtyChunk(chunk + ivec3(-1, 0, 0));
	if (lx == (CHUNK_XYZ - 1))
		AddDirtyChunk(chunk + ivec3(1, 0, 0));
	if (ly == 0)
		AddDirtyChunk(chunk + ivec3(0, -1, 0));
	if (ly == (CHUNK_XYZ - 1))
		AddDirtyChunk(chunk + ivec3(0, 1, 0));
	if (lz == 0)
		AddDirtyChunk(chunk + ivec3(0, 0, -1));
	if (lz == (CHUNK_XYZ - 1))
		AddDirtyChunk(chunk + ivec3(0, 0, 1));
}

void ChunkManager::AddDirtyChunk(const ivec3& chunkpos)
{
	if (chunkpos.x < 0 || chunkpos.y < 0 || chunkpos.z < 0)
		return;

	int index = GetChunkIndex(chunkpos);
	auto it = m_Chunks.find(index);
	if (it == m_Chunks.end())
		return;

	Chunk& chunk = it->second;

	if (!chunk.GetDirty()) {
		chunk.SetDirty(true);
		m_DirtyChunks.push(index);
	}
}

int ChunkManager::GetChunkIndex(const ivec3 &chunk)
{
	return (chunk.y * CHUNK_XYZ + chunk.z) * CHUNK_XYZ + chunk.x;
}

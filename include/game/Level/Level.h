#pragma once
#include "Player/Player.h"

#include "Chunk/Chunk.h"

#include <Physics/AABB.h>

using namespace MC;
using namespace Math;
using namespace Physics;
using namespace Graphics;

class Level 
{
public:
	std::unordered_map<int, Chunk> m_ChunkRenderer;
	std::vector<ivec3> m_Updates;
private:
	size_t m_Volume;
	int m_ChunkUpdates;
	int m_DrawCalls;
	ivec3 m_Size;
	mc_str m_LevelFile;
	mc_uc8* m_Blocks;
public:
	Level(const ivec3& size);
	~Level();
public:
	/* check if level file exists */
	bool Levelcheck();
	void Save();
	void Load();
public:
	bool IsSolidTile(const ivec3& pos);
	bool IsLightBlocker(const ivec3& pos);
	float GetBrigthness(const ivec3& pos);
public:
	void Render(Shader* shader, Player* player);
public:
	void SetTile(const ivec3& blockpos, int type);
public:
	std::vector<AABB> GetCubes(const AABB& aabb);
public:
	/* convert chunk position to chunk array */
	int GetChunkIndex(const ivec3& block);

	/* convert block position to position in level array */
	int GetBlockIndex(const ivec3& block);
public:
	inline size_t GetChunksCount() { return m_ChunkRenderer.size(); }
public:
	inline void IncrementDrawCalls() { m_DrawCalls++; }
	inline void RestartDrawCalls() { m_DrawCalls = 0; }
	inline int GetDrawCalls() { return m_DrawCalls; };
public:
	inline void IncrementUpdates() { m_ChunkUpdates++; }
	inline void RestartUpdates() { m_ChunkUpdates = 0; }
	inline int GetUpdates() const { return m_ChunkUpdates; }
	inline ivec3 GetSize() const { return m_Size; }
};
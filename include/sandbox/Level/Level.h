#pragma once
#include "Player/Player.h"

#include <Physics/AABB.h>

using namespace MC;
using namespace Math;
using namespace Physics;
using namespace Graphics;

class Chunk;

class Level 
{
public:
	std::vector<ivec3> m_Updates;
	std::unordered_map<int, Chunk> m_ChunkRenderer;
private:
	int m_ChunkUpdates;
	int m_DrawCalls;
	uint8_t* m_Blocks;
	ivec3 m_Size;
	rd_str_t m_LevelFile;
public:
	Level(const ivec3& size);
	~Level();
public:
	bool Levelcheck();
	void Save();
	void Load();
public:
	bool IsSolidTile(ivec3 pos);
	bool IsLightBlocker(ivec3 pos);
	float GetBrigthness(ivec3 pos);
public:
	void Render(Shader* shader, Player* player);
public:
	void SetTile(ivec3 blockpos, int type);
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
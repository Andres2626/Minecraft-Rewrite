#pragma once
#include "PerlinNoiseFilter.h"

#include "Player/Player.h"

#include "Chunk/Chunk.h"
#include "Chunk/ChunkManager.h"

#include <Physics/AABB.h>

using namespace MC;
using namespace Math;
using namespace Physics;
using namespace Graphics;

class ChunkManager;

class Level 
{
protected:
	std::unique_ptr<ChunkManager> m_ChunkManager;
private:
	size_t m_Volume;
	ivec3 m_Size;
	mc_str m_LevelFile;
	u8t *m_Blocks;
	u8t *m_HeightMap;
public:
	Level(const ivec3& size);
	~Level();
public:
	/* check if level file exists */
	bool Levelcheck();
	void Save();
	void Load();
	void BuildMap();
public:
	void UpdateHeightMap(int x, int z);
	bool IsSolidTile(const ivec3& pos);
	bool IsLightBlocker(const ivec3& pos);
	bool IsLit(const ivec3 &pos);
	bool IsOutOfBounds(const ivec3 &pos);
	float GetBrigthness(const ivec3& pos);
public:
	void Render(Shader* shader, Player* player);
	void Update();
	void Tick();
public:
	void SetTile(const ivec3& blockpos, BlockType type);
	BlockType GetBlockType(const ivec3& pos);
public:
	std::vector<AABB> GetCubes(const AABB& aabb);
public:
	int GetBlockIndex(const ivec3& block);
	inline ivec3 GetSize() const { return m_Size; }
	ChunkManager *GetChunkManager() const;
};
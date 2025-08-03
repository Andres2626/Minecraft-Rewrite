#pragma once
#include "Player/Player.h"

#include <Physics/AABB.h>

using namespace MC;
using namespace Math;
using namespace Physics;
using namespace Graphics;

class Chunk;

class Level {
public:
	std::vector<ivec3> ChunkUpdates;
	std::unordered_map<int, Chunk> chunks;
	std::string m_LevelFile;
private:
	int m_ChunkUpdates;
public:
	uint8_t* blocks;
	ivec3 size;
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
	int GetChunkIndex(const ivec3& block);
	int GetBlockIndex(const ivec3& block);
public:
	inline void RestartUpdates() { this->m_ChunkUpdates = 0; }
	inline int GetUpdates() const { return this->m_ChunkUpdates; }
};
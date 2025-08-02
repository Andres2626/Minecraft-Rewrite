#pragma once
#include "Player/Player.h"

#include <Physics/AABB.h>

class Chunk;

class Level {
public:
	std::vector<glm::ivec3> ChunkUpdates;
	std::unordered_map<int, Chunk> chunks;
	std::string m_LevelFile;
private:
	int m_ChunkUpdates;
public:
	uint8_t* blocks;
	glm::ivec3 size;
public:
	Level(const glm::ivec3& size);
	~Level();
public:
	bool Levelcheck();
	void Save();
	void Load();
public:
	bool IsSolidTile(glm::ivec3 pos);
	bool IsLightBlocker(glm::ivec3 pos);
	float GetBrigthness(glm::ivec3 pos);
public:
	void Render(MC::Graphics::Shader* shader, Player* player);
public:
	void SetTile(glm::ivec3 blockpos, int type);
public:
	std::vector<MC::Physics::AABB> GetCubes(const MC::Physics::AABB& aabb);
public:
	int GetChunkIndex(const glm::ivec3& block);
	int GetBlockIndex(const glm::ivec3& block);
public:
	inline void RestartUpdates() { this->m_ChunkUpdates = 0; }
	inline int GetUpdates() const { return this->m_ChunkUpdates; }
};
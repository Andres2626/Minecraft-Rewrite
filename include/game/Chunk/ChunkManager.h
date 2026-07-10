#pragma once
#include "ChunkDef.h"
#include "Chunk/Chunk.h"

#include <queue>

class Level;
class Player;

class ChunkManager
{
protected:
	Level *m_Level;
private:
	std::queue<int> m_DirtyChunks;
	std::unordered_map<int, Chunk> m_Chunks;
	int m_ChunkUpdates;
	int m_DrawCalls;
public:
	ChunkManager(Level* lev);
	~ChunkManager();
public:
	void Create();
	void Render(Shader *shader, Player *player);
	void Update();
	void Tick();
	void MarkDirty(const ivec3 &blockpos);
	void AddDirtyChunk(const ivec3 &chunkpos);
public:
	inline void IncrementDrawCalls() { m_DrawCalls++; }
	inline void RestartDrawCalls() { m_DrawCalls = 0; }
	inline int GetChunkUpdates() { return m_ChunkUpdates; }
	inline int GetDrawCalls() { return m_DrawCalls; }
	inline size_t GetChunksCount() { return m_Chunks.size(); }
public:
	int GetChunkIndex(const ivec3& block);
};
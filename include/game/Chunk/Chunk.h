#pragma once
#include "ChunkDef.h"

#include "Block/BlockManager.h"

#include <Physics/AABB.h>
#include <Graphics/Mesh.h>
#include <Graphics/Shader/Shader.h>
#include <Graphics/MeshFactory.h>
#include <Math/Math.h>

#include <vector>

using namespace MC;
using namespace Math;
using namespace Graphics;
using namespace Physics;

class Level;

class Chunk 
{
protected:
	std::unique_ptr<Mesh> m_Mesh;
	Level *m_Level;
	MeshData m_MeshData;
private:
	/* rebuild flag */
	bool m_Dirty; 
	bool m_Queued;
	u32t m_Priority;
	u32t m_DirtyTick;
	ivec3 m_Pos;
	AABB m_Box;
public:
	Chunk(Level *level, const ivec3 &pos);
	~Chunk();
public:
	/* calculate mesh */
	void Build();
	void Render() const;
private:
	void AddFace(const ivec3 &fpos, Face f, Block &t);
	void AddQuad(const vec2& uv, const ivec3& pos, const ivec3* quad, const float brightness);
public:
	inline void SetDirty(bool dirty) { m_Dirty = dirty; }
	inline bool GetDirty() { return m_Dirty; }
	inline void SetQueued(bool queued) { m_Queued = queued; }
	inline bool GetQueued() { return m_Queued; }
	inline void SetPriority(u32t priority) { m_Priority = priority; }
	inline u32t GetPriority() { return m_Priority; }
	inline void SetDirtyTick(u32t tick) { m_DirtyTick = tick; }
	inline u32t GetDirtyTick() { return m_DirtyTick; }
	inline ivec3 &GetPosition() { return m_Pos; }
	inline AABB &GetBox() { return m_Box; }
};
#pragma once
#include "ChunkDef.h"
#include "ChunkMesh.h"

#include "Block/BlockManager.h"
#include "Model/BushModel.h"

#include <Physics/AABB.h>
#include <Graphics/Mesh.h>
#include <Graphics/Shader/Shader.h>
#include <Graphics/MeshFactory.h>
#include <Math/Math.h>

#include <vector>
#include <memory>

using namespace MC;
using namespace Math;
using namespace Graphics;
using namespace Physics;

class Level;

class Chunk : public NonCopyable, public NonMovable
{
protected:
	ChunkMesh m_ChunkMesh;
	Level *m_Level;
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
public:
	inline void SetDirty(bool dirty) { m_Dirty = dirty; }
	inline bool GetDirty() { return m_Dirty; }
	inline void SetQueued(bool queued) { m_Queued = queued; }
	inline bool GetQueued() { return m_Queued; }
	inline bool NeedsBuild() { return m_Dirty && !m_Queued; };
	inline ivec3 &GetPosition() { return m_Pos; }
	inline AABB &GetBox() { return m_Box; }
	inline std::vector<StaticBlockInstance> &GetBushInstances() { return m_ChunkMesh.GetBushInstances(); }
};
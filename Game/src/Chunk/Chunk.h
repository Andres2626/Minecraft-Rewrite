#pragma once
#include "ChunkDef.h"

#include "Block/Tile.h"

#include <Physics/AABB.h>
#include <Graphics/Shader/Shader.h>
#include <Graphics/Buffers/IndexBuffer.h>
#include <Graphics/Buffers/VertexArray.h>
#include <Graphics/Buffers/VertexBuffer.h>
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
	std::unique_ptr<VertexArray> VAO;
	std::unique_ptr<VertexBuffer> VBO;
	std::unique_ptr<IndexBuffer> IBO;
	Level* m_Level;
public:
	/* VBO buffer */
	std::vector<float> vertices;

	/* IBO buffer */
	std::vector<unsigned int> indices;
private:
	bool m_Dirty; 
	ivec3 m_Pos;
	AABB m_Box;
public:
	Chunk(Level* level, const ivec3& pos);
	~Chunk();
public:
	/* calculate mesh */
	void Build();

	/* draw chunk */
	void Render(Shader* shader) const;
private:
	void AddFace(const vec3& fpos, Face f, Tile t);
	void PushIndices(int count);
public:
	inline void SetDirty() { this->m_Dirty = true; }
	inline bool GetDirty() { return this->m_Dirty; }
	inline ivec3 GetPosition() { return this->m_Pos; }
	inline AABB GetBox() { return this->m_Box; }
};
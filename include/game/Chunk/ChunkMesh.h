#pragma once

#include "Block/Block.h"
#include "Renderer/StaticModelRenderer.h"

#include <Graphics/Mesh.h>

using namespace MC;
using namespace Math;
using namespace Graphics;

class Level;

class ChunkMesh : public Mesh
{
private:
	std::vector<StaticBlockInstance> m_BushInstances;
public:
	ChunkMesh();
	~ChunkMesh();
public:
	static void Init();
	void Build(Level *lev, const vec3 &pos);
public:
	void Render() const;
private:
	void AddFace(Level *lev, MeshData &buff, const ivec3 &fpos, Face f, Block &t);
	void AddModelInstance(const ivec3 &pos, BlockType ty, const float brightness);
public:
	inline std::vector<StaticBlockInstance> &GetBushInstances() { return m_BushInstances; }
};
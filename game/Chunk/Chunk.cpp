#include "Chunk/Chunk.h"

#include "Level/Level.h"
#include "Model/ModelManager.h"
#include "Model/BlockModel.h"

#include <Graphics/Shader/Shaderflags.h>
#include <Graphics/Renderer.h>
#include <Utils/Profiler.h>
#include <Utils/Util.h>

Chunk::Chunk(Level *level, const ivec3 &pos)
   : m_Pos(pos)
{
	m_Level = level;
	m_Dirty = true;
	m_Queued = false;

	/* build chunk box */
	m_Box = AABB({ pos.x * CHUNK_XYZ, pos.y * CHUNK_XYZ, pos.z * CHUNK_XYZ }, 
		       { (pos.x * CHUNK_XYZ) + CHUNK_XYZ, (pos.y * CHUNK_XYZ) + CHUNK_XYZ, 
		       (pos.z * CHUNK_XYZ) + CHUNK_XYZ });

	Build();
}

Chunk::~Chunk() 
{

}

void Chunk::Build() 
{
	/* if the chunk is marked as 'dirty', rebuild the vertices and indices */
	if (!m_Dirty)
		return; /* advoid chunk rebuilding */

	m_ChunkMesh.Build(m_Level, m_Pos);

	m_Dirty = false;
}

void Chunk::Render() const
{
	PROFILE_FUNCTION();
	m_ChunkMesh.Render();
}
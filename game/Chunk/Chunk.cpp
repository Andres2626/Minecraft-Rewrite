#include "Chunk/Chunk.h"

#include "Level/Level.h"

#include <Graphics/Shader/Shaderflags.h>
#include <Graphics/Renderer.h>

/* create block */

static std::array<ivec3, 4> front = {
	ivec3(0, 0, 0),
	ivec3(1, 0, 0),
	ivec3(1, 1, 0),
	ivec3(0, 1, 0)
};

static std::array<ivec3, 4> back = {
	ivec3(1, 0, 1),
	ivec3(0, 0, 1),
	ivec3(0, 1, 1),
	ivec3(1, 1, 1)
};

static std::array<ivec3, 4> left = {
	ivec3(0, 0, 0),
	ivec3(0, 0, 1),
	ivec3(0, 1, 1),
	ivec3(0, 1, 0)
};

static std::array<ivec3, 4> right = {
	ivec3(1, 0, 1),
	ivec3(1, 0, 0),
	ivec3(1, 1, 0),
	ivec3(1, 1, 1)
};

static std::array<ivec3, 4> bottom = {
	ivec3(0, 0, 1),
	ivec3(1, 0, 1),
	ivec3(1, 0, 0),
	ivec3(0, 0, 0)
};

static std::array<ivec3, 4> top = {
	ivec3(0, 1, 0),
	ivec3(1, 1, 0),
	ivec3(1, 1, 1),
	ivec3(0, 1, 1)
};

glm::ivec3 FaceNormals[] = {
	{ 0, 0, -1 },
	{ 0, 0, 1 },
	{ -1, 0, 0 },
	{ 1, 0, 0 },
	{ 0, -1, 0 },
	{ 0, 1, 0 }
};

Chunk::Chunk(Level *level, const ivec3 &pos)
   : m_Level(level), m_Pos(pos), m_Dirty(true)
{
	m_Level = level;
	m_Dirty = true;

	/* build chunk box */
	m_Box = AABB({ pos.x * CHUNK_XYZ, pos.y * CHUNK_XYZ, pos.z * CHUNK_XYZ }, 
		       { (pos.x * CHUNK_XYZ) + CHUNK_XYZ, (pos.y * CHUNK_XYZ) + CHUNK_XYZ, 
		       (pos.z * CHUNK_XYZ) + CHUNK_XYZ });

	m_Mesh = std::make_unique<Mesh>(&m_MeshData);	

	/* build buffer mesh */
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

	/* reset indices and vertices vector */
	m_MeshData.vertices.clear();
	m_MeshData.indices.clear();

	for (int cx = 0; cx < CHUNK_XYZ; cx++) {
		for (int cy = 0; cy < CHUNK_XYZ; cy++) {
			for (int cz = 0;cz < CHUNK_XYZ; cz++) {

				/* convert chunk position to global position */
				ivec3 p((m_Pos.x * CHUNK_XYZ) + cx, (m_Pos.y * CHUNK_XYZ) + cy,
					   (m_Pos.z * CHUNK_XYZ) + cz);

				BlockType ty = m_Level->GetBlockType(p);
				if (ty == BlockType::AIR)
					continue; /* ignore air blocks */

				Block &blk = BlockManager::GetBlockType(ty);

				/*
				 * Push all the vertices in the buffer.
				 * ignore tile meshing if have adjacent solid tile.
				 */
				if (m_Level->IsSolidTile(p)) {
					for (int i = 0; i < 6; i++) {
						if (!m_Level->IsSolidTile(p + FaceNormals[i]))
							AddFace(p, (Face)i, blk);
					}
				}
			}
		}
	}

	m_Mesh->Build();
	m_Dirty = false;
}

void Chunk::Render(Shader *shader) const
{  
	/* push model matrix */
	shader->Set4x4("s_M", mat4(1.0f));
	m_Level->GetChunkManager()->IncrementDrawCalls();
	m_Mesh->Render();
}

void Chunk::AddFace(const ivec3 &fpos, Face f, Block &t) 
{
	int count = (int)m_MeshData.vertices.size();

	/* brigthness face values */
	float c1 = 1.0f;
	float c2 = 0.8f;
	float c3 = 0.6f;
	float br = 0.0f;
	vec2 uv = { 0.0f, 0.0f };
	ivec3 *quad;

	switch (f)
	{
	case Face::FRONT:
		quad = front.data();
		uv = t.front;
		br = m_Level->GetBrigthness({ fpos.x, fpos.y, fpos.z - 1 }) * c2;
		break;
	case Face::BACK:
		quad = back.data();
		uv = t.back;
		br = m_Level->GetBrigthness({ fpos.x, fpos.y, fpos.z + 1 }) * c2;
		break;
	case Face::LEFT:
		quad = left.data();
		uv = t.left;
		br = m_Level->GetBrigthness({ fpos.x - 1, fpos.y, fpos.z }) * c3;
		break;
	case Face::RIGHT:
		quad = right.data();
		uv = t.right;
		br = m_Level->GetBrigthness({ fpos.x + 1, fpos.y, fpos.z }) * c3;
		break;
	case Face::BOTTOM:
		quad = bottom.data();
		uv = t.bottom;
		br = m_Level->GetBrigthness({ fpos.x, fpos.y - 1, fpos.z }) * c1;
		break;
	case Face::TOP:
		quad = top.data();
		uv = t.top;
		br = m_Level->GetBrigthness({ fpos.x, fpos.y + 1, fpos.z }) * c1;
		break;
	default:
		return;
	}

	AddQuad(uv, fpos, quad, br);

	MeshFactory::AddIndices(m_MeshData, count);
}

void Chunk::AddQuad(const vec2 &uv, const ivec3 &pos, const ivec3 *quad, const float brightness)
{
	vec2 uvs[] = {
		uv + vec2(0, UV_COORD),
		uv + vec2(UV_COORD, UV_COORD),
		uv + vec2(UV_COORD, 0),
		uv
	};

	vec3 color = vec3(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < 4; i++)
	{
		m_MeshData.vertices.push_back({quad[i] + pos, color, uvs[i], brightness});
	}
}

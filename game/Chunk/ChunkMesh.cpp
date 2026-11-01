#include "Chunk/ChunkMesh.h"

#include "Chunk/ChunkDef.h"

#include "Block/BlockManager.h"
#include "Level/Level.h"
#include "Model/BlockModel.h"
#include "Model/ModelManager.h"

#include <Graphics/Shader/Shaderflags.h>

static VertexLayout s_VL;
static bool s_VLInitialized = false;

static void InitVertexLayout()
{
	if (s_VLInitialized)
		return;

	s_VL.AddAttribute<Math::vec3>(SHADER_VERTEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	s_VL.AddAttribute<Math::vec3>(SHADER_COLOR_BIT, sizeof(Vertex), (void*)offsetof(Vertex, color));
	s_VL.AddAttribute<Math::vec2>(SHADER_TEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	s_VL.AddAttribute<float>(SHADER_BRIG_BIT, sizeof(Vertex), (void*)offsetof(Vertex, brig));

	s_VLInitialized = true;
}

ChunkMesh::ChunkMesh()
	: Mesh(s_VL)
{

}

ChunkMesh::~ChunkMesh()
{

}

void ChunkMesh::Init()
{
	InitVertexLayout();
}

void ChunkMesh::Build(Level *lev, const vec3 &pos)
{
	MeshData buff;
	m_BushInstances.clear();

	BlockModel &model = static_cast<BlockModel&>(ModelManager::GetModelType(ModelType::BLOCK));

	for (int cx = 0; cx < CHUNK_XYZ; cx++) {
		for (int cy = 0; cy < CHUNK_XYZ; cy++) {
			for (int cz = 0; cz < CHUNK_XYZ; cz++) {

				/* convert chunk position to global position */
				ivec3 p((pos.x * CHUNK_XYZ) + cx, (pos.y * CHUNK_XYZ) + cy,
					(pos.z * CHUNK_XYZ) + cz);

				BlockType ty = lev->GetBlockType(p);
				Block& blk = BlockManager::GetBlockType(ty);
				if (ty == BlockType::AIR)
					continue; /* ignore air blocks */

				/*
				 * Push all the vertices in the buffer.
				 * ignore tile meshing if have adjacent solid tile.
				 */
				if (lev->IsSolidTile(p)) {
					for (int i = 0; i < 6; i++) {
						const ivec3& normal = model.GetBlockFaceNormals((Face)i);
						if (!lev->IsSolidTile(p + normal))
							AddFace(lev, buff, p, (Face)i, blk);
					}
				}
				else if (blk.GetBlockFlags().RenderInstance)
					AddModelInstance(p, ty, lev->GetBrigthness(p));
			}
		}
	}

	if (buff.indices.empty() || buff.vertices.empty())
		return;

	Mesh::Bind();
	Mesh::Build(&buff);
	Mesh::Unbind();
}

void ChunkMesh::Render() const
{
	Mesh::Render();
}

void ChunkMesh::AddFace(Level *lev, MeshData &buff, const ivec3 &fpos, Face f, Block& t)
{
	mc_assert(((int)f >= 0 && (int)f <= 5), "invalid block face");

	int count = (int)buff.vertices.size();

	/* brigthness face values */
	float c1 = 1.0f;
	float c2 = 0.8f;
	float c3 = 0.6f;
	float br = 0.0f;
	vec2 uv = { 0.0f, 0.0f };
	ivec3* quad;

	BlockModel& model = static_cast<BlockModel&>(ModelManager::GetModelType(ModelType::BLOCK));
	quad = model.GetBlockFace(f);

	switch (f) {
	case Face::FRONT:
		uv = t.front;
		br = lev->GetBrigthness({ fpos.x, fpos.y, fpos.z - 1 }) * c2;
		break;
	case Face::BACK:
		uv = t.back;
		br = lev->GetBrigthness({ fpos.x, fpos.y, fpos.z + 1 }) * c2;
		break;
	case Face::LEFT:
		uv = t.left;
		br = lev->GetBrigthness({ fpos.x - 1, fpos.y, fpos.z }) * c3;
		break;
	case Face::RIGHT:
		uv = t.right;
		br = lev->GetBrigthness({ fpos.x + 1, fpos.y, fpos.z }) * c3;
		break;
	case Face::BOTTOM:
		uv = t.bottom;
		br = lev->GetBrigthness({ fpos.x, fpos.y - 1, fpos.z }) * c1;
		break;
	case Face::TOP:
		uv = t.top;
		br = lev->GetBrigthness({ fpos.x, fpos.y + 1, fpos.z }) * c1;
		break;
	default:
		return;
	}

	vec2 uvs[] = {
		uv + vec2(0, UV_COORD),
		uv + vec2(UV_COORD, UV_COORD),
		uv + vec2(UV_COORD, 0),
		uv
	};

	Vertex v0 = { quad[0] + fpos, vec3(1.0f), uvs[0], br };
	Vertex v1 = { quad[1] + fpos, vec3(1.0f), uvs[1], br };
	Vertex v2 = { quad[2] + fpos, vec3(1.0f), uvs[2], br };
	Vertex v3 = { quad[3] + fpos, vec3(1.0f), uvs[3], br };

	MeshFactory::AddQuad(buff, v0, v1, v2, v3);
	MeshFactory::AddIndices(buff, count);
}

void ChunkMesh::AddModelInstance(const ivec3 &pos, BlockType ty, const float brightness)
{
	switch (ty) {
	case BlockType::BUSH:
		m_BushInstances.push_back({ pos, brightness });
		break;
	default:
		mc_assert(false, "not implemented block instance");
		break;
	}
}
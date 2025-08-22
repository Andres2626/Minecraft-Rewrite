#include "Level/Level.h"

#include "Chunk/Chunk.h"
#include "Player/Player.h"

#include <Utils/Util.h>
#include <Utils/gzip.h>

#include <memory>

Level::Level(const ivec3& size)
	: m_Size(size), m_ChunkUpdates(0), m_LevelFile("level.dat")
{
	/* calculate level volume */
	m_Volume = (size_t)size.x * size.y * size.z;

	/* check valid level size */
	if (m_Volume < 0)
		mc_fatal("level invalid size (x=%i, y=%i, z=%i)\n", size.x, size.y, size.z);

	/* create level array */
	m_Blocks = (mc_uc8*)malloc(m_Volume);
	if (!m_Blocks)
		mc_fatal("alloc_blocks() failed: out of memory (volume=%i)\n", m_Volume);

	/* check if level file exits */
	if (!Levelcheck()) {
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.z; y++) {
				for (int z = 0; z < size.y; z++) {
					int index = GetBlockIndex({ x, y, z });
					int solid = (mc_uc8)(y <= (size.z * 2 / 3));
					m_Blocks[index] = solid ? 1 : 0;
				}
			}
		}

		/* create level.dat */
		Save();
	}
	else {
		/* 
		* if the level.dat is found, load direcly instead of build the blocks, 
		* load the file and copy buffer to blocks, as was done in the original 
		* version of Java.
		*/
		Load();
	}

	/* create chunk renderer */
	for (int cx = 0; cx < size.x / CHUNK_XYZ; cx++) {
		for (int cy = 0; cy < size.z / CHUNK_XYZ; cy++) {
			for (int cz = 0; cz < size.y / CHUNK_XYZ; cz++) {
				int index = GetChunkIndex({ cx, cy, cz });

				/*
				 * for avoid call to destructor when chunk inserting in the
				 * map use this instead of map::emplace(index chunk{...})
				 */
				m_ChunkRenderer.emplace(std::piecewise_construct,
					                    std::forward_as_tuple(index),
					                    std::forward_as_tuple(this, ivec3{ cx, cy, cz }));
			}
		}
	}
}

bool Level::IsSolidTile(const ivec3& pos)
{
	return (pos.x >= 0) && (pos.y >= 0) && (pos.z >= 0) &&
		   (pos.x < m_Size.x) && (pos.y < m_Size.z) && (pos.z < m_Size.y) &&
		   m_Blocks[GetBlockIndex(pos)];
}

bool Level::IsLightBlocker(const ivec3& pos)
{
	return IsSolidTile(pos);
}

float Level::GetBrigthness(const ivec3& pos)
{
	/* if the block is out of bounds, mark the block as light */
	if (pos.x < 0 || pos.y < 0 || pos.z < 0 ||
		pos.x >= m_Size.x || pos.y >= m_Size.z || 
		pos.z >= m_Size.y) {
		return 1.0f; /* light block */
	}

	for (int i = pos.y; i < m_Size.z; i++) {
		if (IsLightBlocker({ pos.x, i, pos.z })) {
			return 0.5f; /* dark block */
		}
	}

	return 1.0f; /* light block */
}

Level::~Level() 
{
	/* delete block array */
	if (m_Blocks)
		free(m_Blocks);
}

bool Level::Levelcheck() 
{
	std::ifstream file(m_LevelFile);
	return file.good(); /* if file exists? */
}

void Level::Save() 
{
	mc_info("saving level\n");

	size_t outSize = 0;

	/* compress data */
	unsigned char* raw = Utils::compress(m_Blocks, m_Volume, &outSize);
	if (!raw)
		mc_fatal("error compressing level\n");

	/* open file in binary write */
	std::ofstream file(m_LevelFile, std::ios::out | std::ios::binary);
	if (!file.is_open()) {
		free(raw);
		mc_fatal("failed to open level file: \"%s\"\n", m_LevelFile);
	}

	/* write compressed data */
	file.write(reinterpret_cast<const char*>(raw), outSize);
	file.close();

	/* desallocate data */
	free(raw);
}

void Level::Load() 
{
	mc_info("loading level\n");

	size_t outSize = 0;

	/* open file for binary read */
	std::ifstream file(m_LevelFile, std::ios::binary);
	if (!file)
		mc_fatal("failed to open level file: \"%s\"\n", m_LevelFile);

	/* decompress data */
	std::vector<unsigned char> in((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	unsigned char* raw = Utils::decompress(in.data(), in.size(), &outSize);
	if (!raw)
		mc_fatal("error decompressing level\n");

	mc_assert(outSize == m_Volume, "invalid decompressed file size");

	/* copy decompressed data */
	memcpy(m_Blocks, reinterpret_cast<const char*>(raw), outSize);

	/* desallocate data */
	free(raw);
}

void Level::Render(Shader* shader, Player* player) 
{
	/* check if chunk is in camera frustum */
	for (auto& n : m_ChunkRenderer) {
		if (player->Cam.InFrustum(n.second.GetBox())) {
			n.second.Render(shader);
		}
	}
}

void Level::SetTile(const ivec3& blockpos, int type) 
{
	/* check if the block is out of level */
	if (blockpos.x < 0 || blockpos.y < 0 || blockpos.z < 0 || 
		blockpos.x >= m_Size.x || blockpos.y >= m_Size.z ||
		blockpos.z >= m_Size.y) {
		return; /* block out of bounds */
	}

	/* detect if block is not air */
	bool flag = (type > 0);

	/* set block type in the array */
	int index = GetBlockIndex(blockpos);
	m_Blocks[index] = flag;

	/* convert position to chunk position */
	ivec3 chunk = floor(vec3(blockpos) / (float)CHUNK_XYZ);
	int chunk_index = GetChunkIndex(chunk);

	/* rebuild chunk array */
	std::vector<ivec3>().swap(m_Updates);

	/*
	 * For obtain the CUPS (chunks updates per second) push all
	 * changed chunks in array and update block adjacent blocks
	 */
	m_Updates.push_back(chunk);
	if (blockpos.x % 16 == 0)
		m_Updates.push_back(chunk + ivec3(-1, 0, 0));
	if (blockpos.x % 16 == 15)
		m_Updates.push_back(chunk + ivec3(1, 0, 0));
	if (blockpos.y % 16 == 0)
		m_Updates.push_back(chunk + ivec3(0, -1, 0));
	if (blockpos.y % 16 == 15)
		m_Updates.push_back(chunk + ivec3(0, 1, 0));
	if (blockpos.z % 16 == 0)
		m_Updates.push_back(chunk + ivec3(0, 0, -1));
	if (blockpos.z % 16 == 15)
		m_Updates.push_back(chunk + ivec3(0, 0, 1));


	/* find chunk position in the map */
	for (const auto& ch : m_Updates) {
		chunk_index = GetChunkIndex(ch);
		auto it = m_ChunkRenderer.find(chunk_index);
		if (it != m_ChunkRenderer.end()) {
			/*
			 * yes! the chunk position is found in the level, now the
			 * program check if the chunk is out of level and call to 
			 * SetDirty() and rebuild chunk.
			 */

			if (ch.x < 0 || ch.y < 0 || ch.z < 0)
				continue;

			it->second.SetDirty();
			it->second.Build();
		}
	}

}

std::vector<AABB> Level::GetCubes(const AABB& aabb) 
{
	std::vector <AABB> aabbs;
	vec3 p0 = aabb.p0;
	vec3 p1 = aabb.p1 + 1.0f;
	
	if (p0.x < 0.0f)
		p0.x = 0.0f;

	if (p0.y < 0.0f)
		p0.y = 0.0f;

	if (p0.z < 0.0f)
		p0.z = 0.0f;

	if (p1.x > m_Size.x)
		p1.x = (float)m_Size.x;

	if (p1.y > m_Size.z)
		p1.y = (float)m_Size.z;

	if (p1.z > m_Size.y)
		p1.z = (float)m_Size.y;

	/* check all player adjacent blocks */
	for (int x3 = (int)p0.x; x3 < p1.x; ++x3) {
		for (int y3 = (int)p0.y; y3 < p1.y; ++y3) {
			for (int z3 = (int)p0.z; z3 < p1.z; ++z3) {
				if (IsSolidTile({ x3, y3, z3 })) {
					aabbs.push_back(AABB({ (float)x3, (float)y3, (float)z3 }, { (float)(x3 + 1), (float)(y3 + 1), (float)(z3 + 1) }));
				}
			}
		}
	}

	return aabbs;
}

int Level::GetChunkIndex(const ivec3& chunk) 
{
	return (chunk.y * CHUNK_XYZ + chunk.z) * CHUNK_XYZ + chunk.x;
}

int Level::GetBlockIndex(const ivec3& block) 
{
	return (block.y * m_Size.y + block.z) * m_Size.x + block.x;
}
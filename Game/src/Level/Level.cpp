#include "Level.h"

#include "Chunk/Chunk.h"
#include "Player/Player.h"

#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>

#include <memory>

Level::Level(const ivec3& size)
	: size(size), m_ChunkUpdates(0), m_LevelFile("level.dat")
{
	/* initialize random number generator */
	srand(time(NULL));

	int v = size.x * size.y * size.z;

	/* check valid level size */
	if (v < 0)
		MC_FATAL << "Level invalid size (" << size.x << "," << size.y << "," << size.z << ")";

	/* create level array */
	blocks = (uint8_t*)malloc(v);
	if (!blocks)
		MC_FATAL << "alloc_blocks() failed: out of memory (size=" << v << ")";

	/* check if level.dat exists in filesystem */
	if (!Levelcheck()) {
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.z; y++) {
				for (int z = 0; z < size.y; z++) {
					int index = GetBlockIndex({ x, y, z });
					int solid = (uint8_t)(y <= (size.z * 2 / 3));
					blocks[index] = solid ? 1 : 0;
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
				chunks.emplace(
					std::piecewise_construct,
					std::forward_as_tuple(index),
					std::forward_as_tuple(this, ivec3{ cx, cy, cz })
				);
			}
		}
	}
}

bool Level::IsSolidTile(ivec3 pos)
{
	return (pos.x >= 0) && (pos.y >= 0) && (pos.z >= 0) &&
		(pos.x < size.x) && (pos.y < size.z) && (pos.z < size.y) &&
		(blocks[GetBlockIndex(pos)]);
}

bool Level::IsLightBlocker(ivec3 pos)
{
	return IsSolidTile(pos);
}

float Level::GetBrigthness(ivec3 pos)
{
	/* if the block is out of bounds, mark the block as light */
	if (pos.x < 0 || pos.y < 0 || pos.z < 0 ||
		pos.x >= size.x || pos.y >= size.z || pos.z >= size.y) {
		return 1.0f; /* light block */
	}

	for (int i = pos.y; i < size.z; i++) {
		if (IsLightBlocker({ pos.x, i, pos.z })) {
			return 0.5f; /* dark block */
		}
	}

	return 1.0f; /* light block */
}

Level::~Level() 
{
	/* delete block array */
	free(blocks);
}

bool Level::Levelcheck() 
{
	std::ifstream file(m_LevelFile);
	return file.good(); /* if file exists? */
}

void Level::Save() 
{
	MC_INFO << "Saving level";

	int bufflen = size.x * size.y * size.z;
	std::string buff;

	/* compress file using gzip */
	buff = gzip::compress(reinterpret_cast<const char*>(blocks), bufflen);

	/* open level.dat for write buff */
	std::ofstream dis(m_LevelFile, std::ios::out | std::ios::binary);
	if (!dis.is_open())
		MC_FATAL <<  "Failed to open level file: \"" <<  m_LevelFile << "\"";

	/* write buff to level.dat and close */
	dis.write(buff.data(), buff.size());
	dis.close();
}

void Level::Load() 
{
	MC_INFO << "Loading level";

	/* open level.dat */
	std::ifstream dos(m_LevelFile, std::ios::in | std::ios::binary);
	if (!dos.is_open())
		MC_FATAL << "Failed to open level file: \"" << m_LevelFile << "\"";

	/* read file (compressed) */
	std::string compressed((std::istreambuf_iterator<char>(dos)), std::istreambuf_iterator<char>());
	dos.close();

	/* decompress file using gzip */
	std::string decompressed = gzip::decompress(compressed.c_str(), compressed.size());
	int expectedSize = size.x * size.y * size.z;

	/* copy decompressed file to block array */
	memcpy(blocks, decompressed.data(), expectedSize);
}

void Level::Render(Shader* shader, Player* player) 
{
	/* check if chunk is in camera frustum */
	for (auto& n : chunks) {
		if (player->cam.InFrustum(n.second.GetBox())) {
			n.second.Render(shader);
		}
	}
}

void Level::SetTile(ivec3 blockpos, int type) 
{
	/* check if the block is out of level */
	if (blockpos.x < 0 || blockpos.y < 0 || blockpos.z < 0 || 
		blockpos.x >= size.x || blockpos.y >= size.z || 
		blockpos.z >= size.y) {
		return; /* block out of bounds */
	}

	/* detect if block is not air */
	bool flag = (type > 0);

	/* set block type in the array */
	int index = GetBlockIndex(blockpos);
	blocks[index] = flag;

	/* convert position to chunk position */
	ivec3 chunk = floor(vec3(blockpos) / (float)CHUNK_XYZ);
	int chunk_index = GetChunkIndex(chunk);

	/* rebuild chunk array */
	std::vector<ivec3>().swap(ChunkUpdates);

	/*
	 * For obtain the CUPS (chunks updates per second) push all
	 * changed chunks in array and update block adjacent blocks
	 */
	ChunkUpdates.push_back(chunk);
	if (blockpos.x % 16 == 0)
		ChunkUpdates.push_back(chunk + ivec3(-1, 0, 0));
	if (blockpos.x % 16 == 15)
		ChunkUpdates.push_back(chunk + ivec3(1, 0, 0));
	if (blockpos.y % 16 == 0)
		ChunkUpdates.push_back(chunk + ivec3(0, -1, 0));
	if (blockpos.y % 16 == 15)
		ChunkUpdates.push_back(chunk + ivec3(0, 1, 0));
	if (blockpos.z % 16 == 0)
		ChunkUpdates.push_back(chunk + ivec3(0, 0, -1));
	if (blockpos.z % 16 == 15)
		ChunkUpdates.push_back(chunk + ivec3(0, 0, 1));

	/* count chunk updates */
	m_ChunkUpdates = ChunkUpdates.size();


	/* find chunk position in the map */
	for (const auto& ch : ChunkUpdates) {
		chunk_index = GetChunkIndex(ch);
		auto it = chunks.find(chunk_index);
		if (it != chunks.end()) {
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

	if (p1.x > size.x)
		p1.x = (float)size.x;

	if (p1.y > size.z)
		p1.y = (float)size.z;

	if (p1.z > size.y)
		p1.z = (float)size.y;

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
	/* convert chunk position to chunk array */
	return (chunk.y * CHUNK_XYZ + chunk.z) * CHUNK_XYZ + chunk.x;
}

int Level::GetBlockIndex(const ivec3& block) 
{
	/* convert block position to position in level array */
	return (block.y * size.y + block.z) * size.x + block.x;
}
#include "Level.h"

#include "Chunk/Chunk.h"
#include "Player/Player.h"

#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>

#include <memory>

Level::Level(const ivec3& size)
	: size(size), m_ChunkUpdates(0), m_LevelFile("level.dat")
{
	int v = size.x * size.y * size.z;

	/* check valid level size */
	RD_FATAL_CHK(v > 0) << "Invalid level size.";

	/* create level array */
	this->blocks = (uint8_t*)malloc(v);
	RD_FATAL_CHK(this->blocks) << "alloc_blocks() memory error.";

	/* check if level.dat exists in filesystem */
	if (!this->Levelcheck()) {
		for (int x = 0; x < this->size.x; x++) {
			for (int y = 0; y < this->size.z; y++) {
				for (int z = 0; z < this->size.y; z++) {
					int index = this->GetBlockIndex({ x, y, z });
					int solid = (uint8_t)(y <= (this->size.z * 2 / 3));
					this->blocks[index] = solid ? 1 : 0;
				}
			}
		}

		/* create level.dat */
		this->Save();
	}
	else {
		/* 
		* if the level.dat is found, load direcly instead of build the this->blocks, 
		* load the file and copy buffer to this->blocks, as was done in the original 
		* version of Java.
		*/
		this->Load();
	}

	/* create chunk renderer */
	for (int cx = 0; cx < this->size.x / CHUNK_XYZ; cx++) {
		for (int cy = 0; cy < this->size.z / CHUNK_XYZ; cy++) {
			for (int cz = 0; cz < this->size.y / CHUNK_XYZ; cz++) {
				int index = GetChunkIndex({ cx, cy, cz });

				/*
				 * for avoid call to destructor when chunk inserting in the
				 * map use this instead of map::emplace(index chunk{...})
				 */
				this->chunks.emplace(
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
		(pos.x < this->size.x) && (pos.y < this->size.z) && (pos.z < this->size.y) &&
		(this->blocks[this->GetBlockIndex(pos)]);
}

bool Level::IsLightBlocker(ivec3 pos)
{
	return this->IsSolidTile(pos);
}

float Level::GetBrigthness(ivec3 pos)
{
	/* if the block is out of bounds, mark the block as light */
	if (pos.x < 0 || pos.y < 0 || pos.z < 0 ||
		pos.x >= this->size.x || pos.y >= this->size.z || pos.z >= this->size.y) {
		return 1.0f; /* light block */
	}

	for (int i = pos.y; i < this->size.z; i++) {
		if (this->IsLightBlocker({ pos.x, i, pos.z })) {
			return 0.5f; /* dark block */
		}
	}

	return 1.0f; /* light block */
}

Level::~Level() 
{
	/* delete block array */
	free(this->blocks);
}

bool Level::Levelcheck() 
{
	std::ifstream file(this->m_LevelFile);
	return file.good(); /* if file exists? */
}

void Level::Save() 
{
	RD_INFO << "Saving level.dat...";

	int size = this->size.x * this->size.y * this->size.z;
	std::string buff;

	/* compress file using gzip */
	buff = gzip::compress(reinterpret_cast<const char*>(this->blocks), size);

	/* open level.dat for write buff */
	std::ofstream dis(this->m_LevelFile, std::ios::out | std::ios::binary);
	RD_FATAL_CHK(dis.is_open()) <<  "Could not found level file: ", this->m_LevelFile;

	/* write buff to level.dat and close */
	dis.write(buff.data(), buff.size());
	dis.close();
}

void Level::Load() 
{
	RD_INFO << "Loading level.dat...";

	/* open level.dat */
	std::ifstream dos(this->m_LevelFile, std::ios::in | std::ios::binary);
	RD_FATAL_CHK(dos.is_open()) << "Could not read level file: " << this->m_LevelFile;

	/* read file (compressed) */
	std::string compressed((std::istreambuf_iterator<char>(dos)), std::istreambuf_iterator<char>());
	dos.close();

	/* decompress file using gzip */
	std::string decompressed = gzip::decompress(compressed.c_str(), compressed.size());
	int expectedSize = this->size.x * this->size.y * this->size.z;

	/* copy decompressed file to block array */
	memcpy(this->blocks, decompressed.data(), expectedSize);
}

void Level::Render(Shader* shader, Player* player) 
{
	/* check if chunk is in camera frustum */
	for (auto& n : this->chunks) {
		if (player->cam.InFrustum(n.second.GetBox())) {
			n.second.Render(shader);
		}
	}
}

void Level::SetTile(ivec3 blockpos, int type) 
{
	/* check if the block is out of level */
	if (blockpos.x < 0 || blockpos.y < 0 || blockpos.z < 0 || 
		blockpos.x >= this->size.x || blockpos.y >= this->size.z || 
		blockpos.z >= this->size.y) {
		return; /* block out of bounds */
	}

	/* detect if block is not air */
	bool flag = (type > 0);

	/* set block type in the array */
	int index = GetBlockIndex(blockpos);
	this->blocks[index] = flag;

	/* convert position to chunk position */
	ivec3 chunk = floor(vec3(blockpos) / (float)CHUNK_XYZ);
	int chunk_index = this->GetChunkIndex(chunk);

	/* rebuild chunk array */
	std::vector<ivec3>().swap(this->ChunkUpdates);

	/*
	 * For obtain the CUPS (chunks updates per second) push all
	 * changed chunks in array and update block adjacent blocks
	 */
	this->ChunkUpdates.push_back(chunk);
	if (blockpos.x % 16 == 0)
		this->ChunkUpdates.push_back(chunk + ivec3(-1, 0, 0));
	if (blockpos.x % 16 == 15)
		this->ChunkUpdates.push_back(chunk + ivec3(1, 0, 0));
	if (blockpos.y % 16 == 0)
		this->ChunkUpdates.push_back(chunk + ivec3(0, -1, 0));
	if (blockpos.y % 16 == 15)
		this->ChunkUpdates.push_back(chunk + ivec3(0, 1, 0));
	if (blockpos.z % 16 == 0)
		this->ChunkUpdates.push_back(chunk + ivec3(0, 0, -1));
	if (blockpos.z % 16 == 15)
		this->ChunkUpdates.push_back(chunk + ivec3(0, 0, 1));

	/* count chunk updates */
	this->m_ChunkUpdates = this->ChunkUpdates.size();


	/* find chunk position in the map */
	for (const auto& ch : this->ChunkUpdates) {
		chunk_index = this->GetChunkIndex(ch);
		auto it = this->chunks.find(chunk_index);
		if (it != this->chunks.end()) {
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

	if (p1.x > this->size.x)
		p1.x = (float)this->size.x;

	if (p1.y > this->size.z)
		p1.y = (float)this->size.z;

	if (p1.z > this->size.y)
		p1.z = (float)this->size.y;

	/* check all player adjacent blocks */
	for (int x3 = (int)p0.x; x3 < p1.x; ++x3) {
		for (int y3 = (int)p0.y; y3 < p1.y; ++y3) {
			for (int z3 = (int)p0.z; z3 < p1.z; ++z3) {
				if (this->IsSolidTile({ x3, y3, z3 })) {
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
	return (block.y * this->size.y + block.z) * this->size.x + block.x;
}
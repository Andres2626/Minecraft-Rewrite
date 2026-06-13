#include "Level/Level.h"

#include "Chunk/Chunk.h"
#include "Player/Player.h"

#include <Utils/Util.h>
#include <Utils/gzip.h>

#define MC_LOG_PREFIX "Level"
#include <Log/Log.h>

#include <memory>

static float NormRand()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

Level::Level(const ivec3 &size)
	: m_Size(size), m_ChunkUpdates(0), m_LevelFile("level.dat")
{
	m_Volume = (size_t)(size.x * size.z * size.y);
	if (!m_Volume)
		mc_fatal("invalid size x={} y={} z={}", size.x, size.z, size.y);

	/* create level array */
	m_Blocks = (mc_uc8*)malloc(m_Volume);
	if (!m_Blocks)
		mc_fatal("block array not allocated: {} size={}", m_Volume, strerror(errno));

	/* check if level file exits */
	if (!Levelcheck()) {
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {
				for (int z = 0; z < size.z; z++) {
					int index = GetBlockIndex({ x, y, z });
					int solid = (mc_uc8)(y <= (size.y * 2 / 3));
					m_Blocks[index] = solid ? 1 : 0;
				}
			}
		}

		/* 
		 * create caves
		 * from: https://github.com/thecodeofnotch/rd-131655/blob/master/src/main/java/com/mojang/rubydung/level/Level.java
		 */
		for (int i = 0; i < 10000; i++) {
			int caveSize = (int)(NormRand() * 7) + 1;

			int caveX = (int)(NormRand() * m_Size.x);
			int caveY = (int)(NormRand() * m_Size.y);
			int caveZ = (int)(NormRand() * m_Size.z);

			for (int radius = 0; radius < caveSize; radius++) {
				for (int sphere = 0; sphere < 1000; sphere++) {
					int offsetX = (int)(NormRand() * radius * 2 - radius);
					int offsetY = (int)(NormRand() * radius * 2 - radius);
					int offsetZ = (int)(NormRand() * radius * 2 - radius);

					double distance = offsetX * offsetX + offsetY * offsetY + offsetZ * offsetZ;
					if (distance > radius * radius)
						continue;

					int tileX = caveX + offsetX;
					int tileY = caveY + offsetY;
					int tileZ = caveZ + offsetZ;

					int index = GetBlockIndex({ tileX, tileY, tileZ });

					if (index >= 0 && index < m_Volume) {
						if (tileX > 0 && tileY > 0 && tileZ > 0
							&& tileX < m_Size.x - 1 && tileY < m_Size.y && tileZ < m_Size.z - 1) {

							m_Blocks[index] = 0;
						}
					}
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
		for (int cy = 0; cy < size.y / CHUNK_XYZ; cy++) {
			for (int cz = 0; cz < size.z / CHUNK_XYZ; cz++) {
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

	mc_info("save_file=./{} x={} y={} z={} size={} renderer_size={}", m_LevelFile, size.x, 
		    size.z, size.y, m_Volume, m_ChunkRenderer.size());
}

bool Level::IsSolidTile(const ivec3& pos)
{
	return (pos.x >= 0) && (pos.y >= 0) && (pos.z >= 0) &&
		   (pos.x < m_Size.x) && (pos.y < m_Size.y) && (pos.z < m_Size.z) &&
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
		pos.x >= m_Size.x || pos.y >= m_Size.y || 
		pos.z >= m_Size.z) {
		return 1.0f; /* light block */
	}

	for (int i = pos.y; i < m_Size.y; i++) {
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
	mc_info("saving file=./{}", m_LevelFile);

	size_t outSize = 0;

	/* compress data */
	unsigned char* raw = Utils::compress(m_Blocks, m_Volume, &outSize);
	if (!raw)
		mc_fatal("error compressing");

	/* open file in binary write */
	std::ofstream file(m_LevelFile, std::ios::out | std::ios::binary);
	if (!file.is_open()) {
		free(raw);
		mc_fatal("failed to open file: \"{}\"", m_LevelFile);
	}

	/* write compressed data */
	file.write(reinterpret_cast<const char*>(raw), outSize);
	file.close();

	/* desallocate data */
	free(raw);
}

void Level::Load() 
{
	mc_info("loading level file=./{}", m_LevelFile);

	size_t outSize = 0;

	/* open file for binary read */
	std::ifstream file(m_LevelFile, std::ios::binary);
	if (!file)
		mc_fatal("failed to open file: \"{}\"", m_LevelFile);

	/* decompress data */
	std::vector<unsigned char> in((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	unsigned char* raw = Utils::decompress(in.data(), in.size(), &outSize);
	if (!raw)
		mc_fatal("error decompressing");

	mc_assert(outSize == m_Volume, "invalid decompressed file size");

	/* copy decompressed data */
	memcpy(m_Blocks, reinterpret_cast<const char*>(raw), outSize);

	/* desallocate data */
	free(raw);
}

void Level::Render(Shader *shader, Player *player) 
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
		blockpos.x >= m_Size.x || blockpos.y >= m_Size.y ||
		blockpos.z >= m_Size.z) {
		return; /* block out of bounds */
	}

	/* set block type in the array */
	int index = GetBlockIndex(blockpos);
	m_Blocks[index] = (type > 0);

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
	int lx = blockpos.x & 15;
	int ly = blockpos.y & 15;
	int lz = blockpos.z & 15;

	if (lx == 0)
		m_Updates.push_back(chunk + ivec3(-1, 0, 0));
	if (lx == 15)
		m_Updates.push_back(chunk + ivec3(1, 0, 0));
	if (ly == 0)
		m_Updates.push_back(chunk + ivec3(0, -1, 0));
	if (ly == 15)
		m_Updates.push_back(chunk + ivec3(0, 1, 0));
	if (lz == 0)
		m_Updates.push_back(chunk + ivec3(0, 0, -1));
	if (lz == 15)
		m_Updates.push_back(chunk + ivec3(0, 0, 1));


	/* find chunk position in the map */
	for (const auto &ch : m_Updates) {
		chunk_index = GetChunkIndex(ch);
		auto it = m_ChunkRenderer.find(chunk_index);
		if (it != m_ChunkRenderer.end()) {

			/*
			 * The chunk position is found in the level, now the
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

std::vector<AABB> Level::GetCubes(const AABB &aabb) 
{
	std::vector<AABB> aabbs;
	vec3 p0 = aabb.p0;
	vec3 p1 = aabb.p1 + 1.0f;

	p0.x = std::max(p0.x, 0.0f);
	p0.y = std::max(p0.y, 0.0f);
	p0.z = std::max(p0.z, 0.0f);
	Math::clamp(p0, vec3(0.0f), vec3(m_Size.x, m_Size.y, m_Size.z));

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
	return (block.y * m_Size.z + block.z)* m_Size.x + block.x;
}
#include "Level/Level.h"

#include "Chunk/Chunk.h"
#include "Player/Player.h"

#include "GameProperties.h"
#include "Block/BlockManager.h"

#include <Utils/Util.h>
#include <Utils/gzip.h>

#define MC_LOG_PREFIX "Level"
#include <Log/Log.h>

static std::mt19937 rng(std::random_device{}());

static int randRange(int min, int max)
{
	return std::uniform_int_distribution<int>(min, max)(rng);
}

Level::Level(const ivec3 &size)
	: m_Size(size), m_LevelFile("level.dat")
{
	m_Volume = (size_t)(size.x * size.z * size.y);
	if (!m_Volume)
		mc_fatal("invalid size x={} y={} z={}", size.x, size.z, size.y);
	
	BlockManager::Init();

	/* create level array */
	m_Blocks = (u8t*)malloc(m_Volume);
	if (!m_Blocks)
		mc_fatal("block array not allocated: {} size={}", m_Volume, strerror(errno));

	m_HeightMap = (u8t*)malloc(size.x * size.z);
	if (!m_HeightMap)
		mc_fatal("heights array not allocated: {} size={}", m_Volume, strerror(errno));

	/* check if level file exits */
	if (!Levelcheck()) {
		/* generate height map with noise algorithm */
		BuildMap();

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

		for (int x = 0; x < m_Size.x; x++) {
			for (int z = 0; z < m_Size.z; z++) {
				UpdateHeightMap(x, z);
			}
		}
	}

	m_Shader = &ShaderManager::Get("chunk");
	m_ChunkManager = std::make_unique<ChunkManager>(this);
	m_ChunkManager->Create();

	mc_info("save_file=./{} x={} y={} z={} size={} renderer_size={}", m_LevelFile, size.x, 
		    size.z, size.y, m_Volume, m_ChunkManager->GetChunksCount());
}

void Level::UpdateHeightMap(int x, int z)
{
	int y = m_Size.y;
	while (y--) {
		if (IsLightBlocker({ x, y, z })) {
			m_HeightMap[x + z * m_Size.x] = y;
			return;
		}
	}

	m_HeightMap[x + z * m_Size.x] = 0;
}

bool Level::IsSolidTile(const ivec3 &pos)
{
	if (IsOutOfBounds(pos))
		return false; 

	return m_Blocks[GetBlockIndex(pos)];
}

bool Level::IsLightBlocker(const ivec3 &pos)
{
	return IsSolidTile(pos);
}

float Level::GetBrigthness(const ivec3 &pos)
{
	return IsLit(pos) ? 1.0f : 0.5f;
}

bool Level::IsLit(const ivec3& pos)
{
	/* if the block is out of bounds, mark the block as light */
	if (IsOutOfBounds(pos))
		return true;

	return pos.y >= m_HeightMap[pos.x + pos.z * m_Size.x];
}

bool Level::IsOutOfBounds(const ivec3 &pos)
{
	if (pos.x < 0 || pos.y < 0 || pos.z < 0 ||
		pos.x >= m_Size.x || pos.y >= m_Size.y ||
		pos.z >= m_Size.z) {
		return true;
	}

	return false;
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
	u8t *raw = Utils::compress(m_Blocks, m_Volume, &outSize);
	if (!raw)
		mc_fatal("error compressing level");

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
		mc_fatal("error decompressing level");

	mc_assert(outSize == m_Volume, "invalid decompressed file size");

	/* copy decompressed data */
	memcpy(m_Blocks, reinterpret_cast<const char*>(raw), outSize);

	/* desallocate data */
	free(raw);
}

void Level::BuildMap()
{
	PerlinNoiseFilter f0(0);
	PerlinNoiseFilter f1(0);
	PerlinNoiseFilter f2(1);
	PerlinNoiseFilter f3(1);

	int *firstmap = (int*)malloc(m_Size.x * m_Size.z * sizeof(int));
	int *secondmap = (int*)malloc(m_Size.x * m_Size.z * sizeof(int));
	int *cliffmap = (int*)malloc(m_Size.x * m_Size.z * sizeof(int));
	int *rockmap = (int*)malloc(m_Size.x * m_Size.z * sizeof(int));
	firstmap = f0.read(m_Size.x, m_Size.z, firstmap);
	secondmap = f1.read(m_Size.x, m_Size.z, secondmap);
	cliffmap = f2.read(m_Size.x, m_Size.z, cliffmap);
	rockmap = f3.read(m_Size.x, m_Size.z, rockmap);

	if (!firstmap || !secondmap || !cliffmap || !rockmap) {
		mc_fatal("Error generating noise map");
		if (firstmap)
			free(firstmap);
		if (secondmap)
			free(secondmap);
		if (cliffmap)
			free(cliffmap);
		if (rockmap)
			free(rockmap);

		exit(-1);
	}

	for (int x = 0; x < m_Size.x; x++) {
		for (int y = 0; y < m_Size.y; y++) {
			for (int z = 0; z < m_Size.z; z++) {
				int idx = x + z * m_Size.x;
				int fHv = firstmap[idx];
				int sHv = secondmap[idx];

				if (cliffmap[idx] < 128)
					sHv = fHv;

				int maxHeight = max(sHv, fHv) / 8 + m_Size.y / 3;
				int maxRockHeight = rockmap[idx] / 8 + m_Size.y / 3;

				m_HeightMap[x + z * m_Size.x] = maxHeight;

				if (maxRockHeight > maxHeight - 2)
					maxRockHeight = maxHeight - 2;

				int index = GetBlockIndex({x, y, z});
				int id = 0;

				if (y == maxHeight)
					id = (int)BlockType::GRASS;

				if (y < maxHeight)
					id = (int)BlockType::DIRT;

				if (y <= maxRockHeight)
					id = (int)BlockType::ROCK;

				m_Blocks[index] = (u8t)id;
			}
		}
	}

	free(firstmap);
	free(secondmap);
	free(cliffmap);
	free(rockmap);
}

void Level::Render(Player *player) 
{
	/*
	 * Actually, this shouldn't go here.
	 * TODO: Create the Render3D, Scene3D and RenderCommand classes.
	 */
	m_Shader->SetVec3("s_cpos", player->Cam.pos);
	m_Shader->SetVec4("s_fcolor0", GlobalGP.fg0.color);
	m_Shader->SetFloat("s_fdensity0", GlobalGP.fg0.density);
	m_Shader->SetVec4("s_fcolor1", GlobalGP.fg1.color);
	m_Shader->SetFloat("s_fdensity1", GlobalGP.fg1.density);
	m_ChunkManager->Render(player);
}

void Level::Update()
{
	m_ChunkManager->Update();

	int totalTiles = m_Volume;
	int ticks = totalTiles / 400;

	for (int i = 0; i < ticks; ++i) {
		ivec3 pos = { randRange(0, m_Size.x), randRange(0, m_Size.y) , randRange(0, m_Size.z) };
		Block &blk = BlockManager::GetBlockType(GetBlockType(pos));
		if (!blk.IsUpdatable())
			continue; /* avoid call update() many times */

		if (blk.GetID() != (BlockType)0)
 			blk.Update(this, pos);
	}
}

void Level::Tick()
{
	m_ChunkManager->Tick();
}

void Level::SetTile(const ivec3& blockpos, BlockType type) 
{
	if (IsOutOfBounds(blockpos))
		return;

	/* set block type in the array */
	int index = GetBlockIndex(blockpos);
	m_Blocks[index] = (int)type;

	UpdateHeightMap(blockpos.x, blockpos.z);
	m_ChunkManager->MarkDirty(blockpos);
}

BlockType Level::GetBlockType(const ivec3 &pos)
{
	if (IsOutOfBounds(pos))
		return BlockType::AIR;

	int index = GetBlockIndex(pos);
	return (BlockType)m_Blocks[index];
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
				if (IsSolidTile({ x3, y3, z3 }))
					aabbs.push_back(AABB({ (float)x3, (float)y3, (float)z3 }, { (float)(x3 + 1), (float)(y3 + 1), (float)(z3 + 1) }));
			}
		}
	}

	return aabbs;
}

int Level::GetBlockIndex(const ivec3& block) 
{
	return (block.y * m_Size.z + block.z)* m_Size.x + block.x;
}

ChunkManager* Level::GetChunkManager() const
{
	return m_ChunkManager.get();
}

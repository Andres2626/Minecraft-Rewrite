#include "Block/Types/Grass.h"

#include "Level/Level.h"

static std::mt19937 rng(std::random_device{}());

static int randRange(int min, int max)
{
	return std::uniform_int_distribution<int>(min, max)(rng);
}

GrassBlock::GrassBlock()
	: Block(BlockType::GRASS)
{
	front = vec2(3 * UV_COORD, 0);
	back = front;
	left = front;
	right = front;
	bottom = vec2(2 * UV_COORD, 0);
	top = vec2(0, 0);

    m_UpdateFlag = 1;
}

GrassBlock::~GrassBlock()
{
}

void GrassBlock::Update(Level *lev, const ivec3 &pos)
{
    Block::Update(lev, pos);

    if (lev->IsLit(pos)) {
        for (int i = 0; i < 4; ++i) {
            vec3 tp = { randRange(0, 3) - 1, randRange(0, 5) - 3, randRange(0, 3) - 1};
            tp += pos;

            if (lev->GetBlockType(tp) == BlockType::DIRT && lev->IsLit(tp))
                lev->SetTile(tp, BlockType::GRASS);
        }
    }
    else
        lev->SetTile(pos, BlockType::DIRT);
}

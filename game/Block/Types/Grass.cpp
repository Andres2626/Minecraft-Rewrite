#include "Block/Types/Grass.h"

#include "Level/Level.h"

GrassBlock::GrassBlock()
	: Block(BlockType::GRASS, 3)
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

void GrassBlock::Update(Level *lev, const ivec3 &pos, Random &random)
{
    Block::Update(lev, pos, random);

    if (lev->IsLit(pos)) {
        for (int i = 0; i < 4; ++i) {
            vec3 tp = { random.NextInt(3) - 1, random.NextInt(5) - 3, random.NextInt(3) - 1 };
            tp += pos;

            if (lev->GetBlockType(tp) == BlockType::DIRT && lev->IsLit(tp))
                lev->SetTile(tp, BlockType::GRASS);
        }
    }
    else
        lev->SetTile(pos, BlockType::DIRT);
}

#include "Block/Types/Rock.h"

RockBlock::RockBlock()
	: Block(BlockType::ROCK)
{
	front = vec2(UV_COORD, 0);
	back = front;
	left = front;
	right = front;
	bottom = front;
	top = front;

	m_UpdateFlag = 0;
}

RockBlock::~RockBlock()
{
}

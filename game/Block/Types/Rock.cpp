#include "Block/Types/Rock.h"

RockBlock::RockBlock()
	: Block(BlockType::ROCK, 1)
{
	front = vec2(UV_COORD, 0);
	back = front;
	left = front;
	right = front;
	bottom = front;
	top = front;

	m_Flags.Collision = 1;
	m_ModelType = ModelType::BLOCK;
}

RockBlock::~RockBlock()
{
}

#include "Block/Types/Dirt.h"

DirtBlock::DirtBlock()
	: Block(BlockType::DIRT, 2)
{
	front = vec2(2 * UV_COORD, 0);
	back = front;
	left = front;
	right = front;
	bottom = front;
	top = front;

	m_Flags.Collision = 1;
	m_ModelType = ModelType::BLOCK;
}

DirtBlock::~DirtBlock()
{
}

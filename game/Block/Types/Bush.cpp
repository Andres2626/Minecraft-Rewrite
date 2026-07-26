#include "Block/Types/Bush.h"

BushBlock::BushBlock()
	: Block(BlockType::BUSH, 15)
{
	front = vec2(15 * UV_COORD, 0);
	back = front;
	left = front;
	right = front;
	bottom = front;
	top = front;

	m_Flags.Collision = 0;
	m_Flags.RenderInstance = 1;
	m_ModelType = ModelType::BUSH;
}

BushBlock::~BushBlock()
{
}

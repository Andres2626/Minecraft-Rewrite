#include "Block/Types/StoneBrick.h"

StoneBrickBlock::StoneBrickBlock()
	: Block(BlockType::STONEBRICK, 16)
{
	front = vec2(0, UV_COORD);
	back = front;
	left = front;
	right = front;
	bottom = front;
	top = front;

	m_UpdateFlag = 0;
}

StoneBrickBlock::~StoneBrickBlock()
{

}

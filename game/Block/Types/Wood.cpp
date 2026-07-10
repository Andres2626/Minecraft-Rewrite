#include "Block/Types/Wood.h"

WoodBlock::WoodBlock()
	: Block(BlockType::WOOD)
{
	front = vec2(4 * UV_COORD, 0);
	back = front;
	left = front;
	right = front;
	bottom = front;
	top = front;

	m_UpdateFlag = 0;
}

WoodBlock::~WoodBlock()
{

}

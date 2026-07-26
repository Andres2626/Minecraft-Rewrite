#include "Block/Types/Wood.h"

WoodBlock::WoodBlock()
	: Block(BlockType::WOOD, 4)
{
	front = vec2(4 * UV_COORD, 0);
	back = front;
	left = front;
	right = front;
	bottom = front;
	top = front;

	m_Flags.Collision = 1;
	m_ModelType = ModelType::BLOCK;
}

WoodBlock::~WoodBlock()
{

}

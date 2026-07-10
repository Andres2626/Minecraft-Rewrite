#include "Block/Block.h"

Block::Block(const BlockType& id)
	: m_ID(id), m_UpdateFlag(0)
{
}

Block::~Block()
{

}

void Block::Update(Level *lev, const ivec3 &pos)
{

}
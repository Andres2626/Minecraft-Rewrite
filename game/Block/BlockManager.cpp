
#include "Block/BlockManager.h"
#include "Block/Types/Blocks.h"

std::array<std::unique_ptr<Block>, 255> BlockManager::m_RegisteredBlocks;

void BlockManager::Init()
{
	Register(std::make_unique<Block>(BlockType::AIR));
	Register(std::make_unique<RockBlock>());
	Register(std::make_unique<GrassBlock>());
	Register(std::make_unique<DirtBlock>());
	Register(std::make_unique<StoneBrickBlock>());
	Register(std::make_unique<WoodBlock>());
}

void BlockManager::Register(std::unique_ptr<Block> block)
{
	m_RegisteredBlocks[(int)block->GetID()] = std::move(block);
}

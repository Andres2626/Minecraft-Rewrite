#pragma once

#include "Block.h"

#include <array>

class BlockManager
{
private:
	static std::array<std::unique_ptr<Block>, 255> m_RegisteredBlocks;
public:
	static void Init();
	static void Register(std::unique_ptr<Block> block);
public:
	static Block &GetBlockType(BlockType type) { return *m_RegisteredBlocks[(int)type].get(); };
};

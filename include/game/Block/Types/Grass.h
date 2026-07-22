#pragma once

#include "Block/Block.h"

class GrassBlock : public Block
{
public:
	GrassBlock();
	~GrassBlock();
public:
	void Update(Level *lev, const ivec3 &pos, Random &random) override;
};
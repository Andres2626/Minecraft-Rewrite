#pragma once

#include "Math/Math.h"
#include "Utils/Random.h"

/*
 * each cell of the texture atlas occupies 1/16 in the UV coordinates.
 * for avoid texture blending use 0.0624375 instead of 0.0625.
 */
#define UV_COORD 0.0624375f

using namespace MC;
using namespace Math;

class ParticleEngine;

enum class BlockType
{
	AIR = 0,
	ROCK = 1,
	GRASS = 2,
	DIRT = 3,
	STONEBRICK = 4,
	WOOD = 5
};

enum class Face
{
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	BOTTOM,
	TOP
};

class Level;

class Block
{
protected:
	BlockType m_ID;
	u32t m_TexID;

	/* 
	 * In general, blocks do not have logic for updating themselves, with a few 
	 * exceptions. What this flag does is skip the update to avoid unnecessarily 
	 * calling `block->Update()` every time `level->Update()` is executed.
	 */
	int m_UpdateFlag;
public:
	vec2 front;
	vec2 back;
	vec2 left;
	vec2 right;
	vec2 bottom;
	vec2 top;
public:
	Block(const BlockType &id, u32t texid);
	~Block();
public:
	virtual void Update(Level *lev, const ivec3 &pos, Random &random);
	virtual void OnDestroy(Level *lev, const ivec3 &pos, ParticleEngine &particleEng);
public:
	BlockType GetID() const { return m_ID; }
	int IsUpdatable() { return m_UpdateFlag; };
};
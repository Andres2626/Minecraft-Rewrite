#pragma once

#include "Model/IModel.h"

#include "Physics/AABB.h"
#include "Math/Math.h"
#include "Utils/Random.h"

/*
 * each cell of the texture atlas occupies 1/16 in the UV coordinates.
 * for avoid texture blending use 0.0624375 instead of 0.0625.
 */
#define UV_COORD 0.0624375f

using namespace MC;
using namespace Math;
using namespace Physics;

class Level;
class EntityManager;

enum class BlockType
{
	AIR = 0,
	ROCK = 1,
	GRASS = 2,
	DIRT = 3,
	STONEBRICK = 4,
	WOOD = 5,
	BUSH = 6
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

struct BlockFlag
{
	/*
	 * In general, blocks do not have logic for updating themselves, with a few
	 * exceptions. What this flag does is skip the update to avoid unnecessarily
	 * calling `block->Update()` every time `level->Update()` is executed.
	 */
	u32t Update : 1;

	/* 
	 * Describe whether the block has collision; many blocks (e.g. bush) 
	 * should not have an AABB. 
	 */
	u32t Collision : 1;

	/* Describe whether the block should render using instancing. */
	u32t RenderInstance : 1;
};

class Block
{
protected:
	BlockType m_ID;
	u32t m_TexID;
	BlockFlag m_Flags;
	ModelType m_ModelType;
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
	virtual void OnDestroy(Level *lev, const ivec3 &pos, EntityManager &entities);
	AABB GetAABB(const ivec3 &pos);
public:
	BlockType GetID() const { return m_ID; }
	u32t GetTexID() const { return m_TexID; };
	BlockFlag GetBlockFlags() const { return m_Flags; };
	ModelType GetModelType() const { return m_ModelType; }
};
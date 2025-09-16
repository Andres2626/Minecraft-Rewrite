#include "Block/Tile.h"

#include <Log/Log.h>

/* 
 * each cell of the texture atlas occupies 1/16 in the UV coordinates.
 * for avoid texture blending use 0.0624375 instead of 0.0625.
 */
#define UV_COORD 0.0624375f

/* 2 values per vertex (u, v)*/
#define UV_SIZE 2

/* 3 values per vertex (x, y, z) */
#define VERTEX_SIZE 3

/* push vertices in the array */
#define VERTEX_FACE(f) v##f[mesh_index] = x; \
v##f[mesh_index + 1] = y; \
v##f[mesh_index + 2] = z;

Tile::Tile(TileType type) 
	: type(type)
{
	/* calculate the UV coordinates from texture atlas */
	switch (type) {
	case TileType::AIR: return; /* UV for air blocks is not nesesary */
	case TileType::GRASS:
		CalculateUV(TileType::GRASS);
		break;
	case TileType::ROCK:
		CalculateUV(TileType::ROCK);
		break;
	default: /* check if the block is valid */
		mc_fatal("invalid block type");
		return; 
	}
	
	/* initialize block array */
	Push(Face::FRONT, 0.0f, 0.0f, 0.0f);
	Push(Face::FRONT, 1.0f, 0.0f, 0.0f);
	Push(Face::FRONT, 1.0f, 1.0f, 0.0f);
	Push(Face::FRONT, 0.0f, 1.0f, 0.0f);
	mesh_index = 0;

	Push(Face::BACK, 0.0f, 0.0f, 1.0f);
	Push(Face::BACK, 1.0f, 0.0f, 1.0f);
	Push(Face::BACK, 1.0f, 1.0f, 1.0f);
	Push(Face::BACK, 0.0f, 1.0f, 1.0f);
	mesh_index = 0;

	Push(Face::LEFT, 0.0f, 1.0f, 1.0f);
	Push(Face::LEFT, 0.0f, 1.0f, 0.0f);
	Push(Face::LEFT, 0.0f, 0.0f, 0.0f);
	Push(Face::LEFT, 0.0f, 0.0f, 1.0f);
	mesh_index = 0;

	Push(Face::RIGHT, 1.0f, 1.0f, 1.0f);
	Push(Face::RIGHT, 1.0f, 1.0f, 0.0f);
	Push(Face::RIGHT, 1.0f, 0.0f, 0.0f);
	Push(Face::RIGHT, 1.0f, 0.0f, 1.0f);
	mesh_index = 0;

	Push(Face::BOTTOM, 0.0f, 0.0f, 0.0f);
	Push(Face::BOTTOM, 1.0f, 0.0f, 0.0f);
	Push(Face::BOTTOM, 1.0f, 0.0f, 1.0f);
	Push(Face::BOTTOM, 0.0f, 0.0f, 1.0f);
	mesh_index = 0;

	Push(Face::TOP, 0.0f, 1.0f, 0.0f);
	Push(Face::TOP, 1.0f, 1.0f, 0.0f);
	Push(Face::TOP, 1.0f, 1.0f, 1.0f);
	Push(Face::TOP, 0.0f, 1.0f, 1.0f);
	mesh_index = 0;
}

Tile::~Tile() 
{

}

void Tile::CalculateUV(TileType type) 
{
	/* calculate UV coordinates */
	float u0 = (int)type / 16.0f;
	float u1 = u0 + UV_COORD;
	float v0 = 1.0f;
	float v1 = v0 - UV_COORD;
	
	Push(u0, v0);
	Push(u1, v0);
	Push(u1, v1);
	Push(u0, v1);
	mesh_index = 0;
}

void Tile::Push(float u, float v) 
{
	texcoords[mesh_index] = u;
	texcoords[mesh_index + 1] = v;
	mesh_index += UV_SIZE;
}

void Tile::Push(Face f, float x, float y, float z) 
{
	switch (f) {
	case Face::FRONT: 
		VERTEX_FACE(front);
		break;
	case Face::BACK: 
		VERTEX_FACE(back);
		break;
	case Face::LEFT: 
		VERTEX_FACE(left);
		break;
	case Face::RIGHT: 
		VERTEX_FACE(right);
		break;
	case Face::BOTTOM: 
		VERTEX_FACE(bottom);
		break;
	case Face::TOP: 
		VERTEX_FACE(top);
		break;
	default:
		mc_fatal("invalid block face");
		return;
	}
	
	mesh_index += VERTEX_SIZE;
}
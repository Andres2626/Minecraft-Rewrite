#include "Block/Tile.h"

static int index = 0;

Tile::Tile(TileType type) 
	: type(type)
{
	/* if the block is air, is not nesesary calculate vertices */
	if (type == AIR)
		return;

	/* calculate the UV coordinates in texture atlas */
	if (type == TileType::GRASS) {
		calculate_tex_coords(TileType::GRASS);
	}

	if (type == TileType::ROCK) {
		calculate_tex_coords(TileType::ROCK);
	}

	/* initialize block model */
	push_vertices(Face::FRONT, 0.0f, 0.0f, 0.0f);
	push_vertices(Face::FRONT, 1.0f, 0.0f, 0.0f);
	push_vertices(Face::FRONT, 1.0f, 1.0f, 0.0f);
	push_vertices(Face::FRONT, 0.0f, 1.0f, 0.0f);
	index = 0;

	push_vertices(Face::BACK, 0.0f, 0.0f, 1.0f);
	push_vertices(Face::BACK, 1.0f, 0.0f, 1.0f);
	push_vertices(Face::BACK, 1.0f, 1.0f, 1.0f);
	push_vertices(Face::BACK, 0.0f, 1.0f, 1.0f);
	index = 0;

	push_vertices(Face::LEFT, 0.0f, 1.0f, 1.0f);
	push_vertices(Face::LEFT, 0.0f, 1.0f, 0.0f);
	push_vertices(Face::LEFT, 0.0f, 0.0f, 0.0f);
	push_vertices(Face::LEFT, 0.0f, 0.0f, 1.0f);
	index = 0;

	push_vertices(Face::RIGHT, 1.0f, 1.0f, 1.0f);
	push_vertices(Face::RIGHT, 1.0f, 1.0f, 0.0f);
	push_vertices(Face::RIGHT, 1.0f, 0.0f, 0.0f);
	push_vertices(Face::RIGHT, 1.0f, 0.0f, 1.0f);
	index = 0;

	push_vertices(Face::BOTTOM, 0.0f, 0.0f, 0.0f);
	push_vertices(Face::BOTTOM, 1.0f, 0.0f, 0.0f);
	push_vertices(Face::BOTTOM, 1.0f, 0.0f, 1.0f);
	push_vertices(Face::BOTTOM, 0.0f, 0.0f, 1.0f);
	index = 0;

	push_vertices(Face::TOP, 0.0f, 1.0f, 0.0f);
	push_vertices(Face::TOP, 1.0f, 1.0f, 0.0f);
	push_vertices(Face::TOP, 1.0f, 1.0f, 1.0f);
	push_vertices(Face::TOP, 0.0f, 1.0f, 1.0f);
	index = 0;
}

Tile::~Tile() 
{

}

void Tile::calculate_tex_coords(TileType type) 
{
	/* calculate uv */
	float u0 = (int)type / 16.0f;
	float u1 = u0 + 0.0624375f; /* constant! */
	float v0 = 1.0f;
	float v1 = v0 - 0.0624375f;
	
	push_uv(u0, v0);
	push_uv(u1, v0);
	push_uv(u1, v1);
	push_uv(u0, v1);
	index = 0;
}

void Tile::push_uv(float u, float v) 
{
	/* push texcoords in texture array */
	texcoords[index] = u;
	texcoords[index + 1] = v;
	index += 2;
}

void Tile::push_vertices(Face f, float x, float y, float z) 
{
	/* push vertices in vertices array */
	if (f == Face::FRONT) {
		vfront[index] = x;
		vfront[index + 1] = y;
		vfront[index + 2] = z;
	}
	else if (f == Face::BACK) {
		vback[index] = x;
		vback[index + 1] = y;
		vback[index + 2] = z;
	}
	else if (f == Face::LEFT) {
		vleft[index] = x;
		vleft[index + 1] = y;
		vleft[index + 2] = z;
	}
	else if (f == Face::RIGHT) {
		vright[index] = x;
		vright[index + 1] = y;
		vright[index + 2] = z;
	}
	else if (f == Face::BOTTOM) {
		vbottom[index] = x;
		vbottom[index + 1] = y;
		vbottom[index + 2] = z;
	}
	else if (f == Face::TOP) {
		vtop[index] = x;
		vtop[index + 1] = y;
		vtop[index + 2] = z;
	}
	index += 3;
}
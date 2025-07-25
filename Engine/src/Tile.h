
#pragma once
#include <memory>

#define TEXCOORD_SIZE 2 * 4
#define VERTICES_SIZE 3 * 4
#define LIGHTS_SIZE 4
#define FACE_SIZE TEXCOORD_SIZE + VERTICES_SIZE + LIGHTS_SIZE

enum class Face {
	FRONT = 1,
	BACK,
	LEFT,
	RIGHT,
	BOTTOM,
	TOP
};

enum TileType {
	AIR = -1,
	GRASS = 0,
	ROCK = 1
};

class Tile {
public:
	TileType type;

	float vfront[VERTICES_SIZE];
	float vback[VERTICES_SIZE];
	float vleft[VERTICES_SIZE];
	float vright[VERTICES_SIZE];
	float vbottom[VERTICES_SIZE];
	float vtop[VERTICES_SIZE];
	float texcoords[TEXCOORD_SIZE];
public:
	Tile(TileType type);
	~Tile();
private:
	void calculate_tex_coords(TileType type);
	void push_uv(float u, float v);
	void push_vertices(enum Face, float x, float y, float z);
};
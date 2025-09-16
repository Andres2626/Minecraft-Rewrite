#pragma once

#define TEXTCOORD_SIZE 2 * 4
#define VERTICES_SIZE 3 * 4
#define LIGHTS_SIZE 4
#define FACE_SIZE TEXTCOORD_SIZE + VERTICES_SIZE + LIGHTS_SIZE

enum class Face 
{
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	BOTTOM,
	TOP
};

enum TileType 
{
	AIR = -1,
	GRASS = 0,
	ROCK = 1
};

class Tile
{
private:
	int mesh_index = 0;
public:
	TileType type;
public:
	float vfront[VERTICES_SIZE];
	float vback[VERTICES_SIZE];
	float vleft[VERTICES_SIZE];
	float vright[VERTICES_SIZE];
	float vbottom[VERTICES_SIZE];
	float vtop[VERTICES_SIZE];
	float texcoords[TEXTCOORD_SIZE];
public:
	Tile(TileType type);
	~Tile();
private:
	void CalculateUV(TileType type);
	void Push(float u, float v);
	void Push(enum Face, float x, float y, float z);
};
#pragma once

#include "Cube.h"

#include <Level/Level.h>
#include <Entity/Entity.h>

#include <Graphics/Buffers/IndexBuffer.h>
#include <Graphics/Buffers/VertexArray.h>
#include <Graphics/Buffers/VertexBuffer.h>

struct ZombiePart
{
	u32t offset;
	u32t size;
	Cube cube;
};

class Zombie : public Entity
{
protected:
	u32t partOffset;
	std::unique_ptr<VertexArray> VAO;
	std::unique_ptr<VertexBuffer> VBO;
	std::unique_ptr<IndexBuffer> IBO;
	std::vector<ZombiePart> parts;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
public:
	float timeoff;
	float rot;
	float rotA;
	float speed;
	float scalefac;
public:
	Zombie(Level &level, vec3 pos);
	~Zombie();
public:
	void Build();
	void Update();
	void Render(float alpha, Shader *shader, float seconds);
	void PushPart(const Cube &cube);
};
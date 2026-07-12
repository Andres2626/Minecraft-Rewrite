#pragma once

#include "ZombieModel.h"

#include <Level/Level.h>
#include <Entity/Entity.h>

#include <Graphics/Shader/ShaderManager.h>
#include <Graphics/Buffers/IndexBuffer.h>
#include <Graphics/Buffers/VertexArray.h>
#include <Graphics/Buffers/VertexBuffer.h>

class Zombie : public Entity
{
public:
	float scalefac;
	float timeoff;
	float rotA;
	float rot;
	float speed;
private:
	vec3 m_PartRot[6];
	Shader *m_Shader;
public:
    Zombie(Level &level, vec3 pos);
    ~Zombie();
public:
    void Update();
};
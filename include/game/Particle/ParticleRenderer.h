#pragma once

#include "Particle.h"
#include "ParticleModel.h"

#include "Player/Player.h"

#include "Graphics/Shader/ShaderManager.h"

using namespace MC;
using namespace Graphics;

class EntityManager;

class ParticleRenderer
{
private:
	EntityManager* m_Entities;
	std::vector<ParticleInstance> m_Instances;
	Shader *m_Shader;
public:
	ParticleRenderer();
	~ParticleRenderer();
public:
	void Render(Player &player, float alpha);
	void SetEntityManager(EntityManager* em);
};
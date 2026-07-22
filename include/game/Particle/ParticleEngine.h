#pragma once

#include "Particle.h"
#include "ParticleModel.h"

#include "Player/Player.h"

#include "Graphics/Shader/ShaderManager.h"

using namespace MC;
using namespace Graphics;

class ParticleEngine
{
private:
	std::vector<ParticleInstance> m_Instances;
	std::vector<std::unique_ptr<Particle>> m_Particles;
	Shader *m_Shader;
public:
	ParticleEngine();
	~ParticleEngine();
public:
	void Add(std::unique_ptr<Particle> particle);
	void Update();
	void Render(Player &player, float alpha);
};
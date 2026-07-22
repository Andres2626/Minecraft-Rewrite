#pragma once

#include "Zombie.h"
#include "ZombieModel.h"

#include "Player/Player.h"
#include "Graphics/Shader/ShaderManager.h"

class EntityManager;

class ZombieRenderer
{
private:
	EntityManager *m_Entities;
	std::vector<ZombieInstance> m_Instances;
	Shader *m_Shader;
public:
	ZombieRenderer();
	~ZombieRenderer();
public:
	void Render(Level &lev, Player &player, float alpha, float seconds);
	void SetEntityManager(EntityManager *em);
};
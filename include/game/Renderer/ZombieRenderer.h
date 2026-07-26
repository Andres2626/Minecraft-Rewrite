#pragma once

#include "Character/Zombie.h"
#include "Graphics/Shader/ShaderManager.h"
#include "Model/ZombieModel.h"
#include "Player/Player.h"

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
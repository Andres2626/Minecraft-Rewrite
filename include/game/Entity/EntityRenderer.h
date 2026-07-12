#pragma once

#include "Entity/EntityManager.h"

#include "Player/Player.h"

#include <Graphics/Shader/ShaderManager.h>
#include <Graphics/Camera/Camera.h>

class EntityRenderer
{
private:
	Shader *m_Shader;
	EntityManager *m_EM;
public:
	EntityRenderer();
	~EntityRenderer();
public:
	void SetEntityManager(EntityManager *ents);
	void Render(Level &lev, Player &player, float alpha, float seconds);
};
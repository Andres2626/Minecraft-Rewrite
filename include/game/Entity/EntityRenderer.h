#pragma once

#include "Entity/EntityManager.h"

#include <Graphics/Camera/Camera.h>

class EntityRenderer
{
private:
	EntityManager *m_EM;
public:
	EntityRenderer();
	~EntityRenderer();
public:
	void SetEntityManager(EntityManager *ents);
	void Render(Level &lev, Camera &cam, Shader *shader, float alpha, float seconds);
};
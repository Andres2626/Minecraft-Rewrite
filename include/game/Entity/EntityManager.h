#pragma once

#include "Entity/Entity.h"

#include "Level/Level.h"

class EntityManager
{
private:
	std::vector<std::unique_ptr<Entity>> m_Entities;
public:
	EntityManager();
	~EntityManager();
public:
	void Update();
	void Register(std::unique_ptr<Entity> ent);
public:
	const std::vector<std::unique_ptr<Entity>> &GetEntities() const { return m_Entities; }
};
#pragma once

#include "Character/Zombie.h"

#include "Level/Level.h"

class EntityManager
{
private:
	std::vector<std::unique_ptr<Zombie>> m_Entities;
public:
	EntityManager();
	~EntityManager();
public:
	void Update();
	void Register(std::unique_ptr<Zombie> ent);
public:
	const std::vector<std::unique_ptr<Zombie>> &GetEntities() const { return m_Entities; }
};
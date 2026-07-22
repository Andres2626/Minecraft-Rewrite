#pragma once

#include "Level/Level.h"

class Particle;
class Zombie;

template<typename T>
struct EntityStorage
{
	std::vector<std::unique_ptr<T>> entities;
};

class EntityManager : public EntityStorage<Zombie>, public EntityStorage<Particle>
{
public:
	EntityManager();
	~EntityManager();
public:
	void Update();

	template<typename T>
	void Register(std::unique_ptr<T> ent) 
	{
		Get<T>().push_back(std::move(ent));
	}
public:
	template<typename T>
	void UpdateStorage(std::vector<std::unique_ptr<T>>& storage)
	{
		auto it = storage.begin();

		while (it != storage.end())
		{
			(*it)->Update();

			if ((*it)->attr.isDead)
				it = storage.erase(it);
			else
				++it;
		}
	}

	template<typename T>
	std::vector<std::unique_ptr<T>>& Get()
	{
		return this->EntityStorage<T>::entities;
	}
};
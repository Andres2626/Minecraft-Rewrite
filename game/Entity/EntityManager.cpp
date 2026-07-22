#include "Entity/EntityManager.h"

#include "Character/Zombie.h"


EntityManager::EntityManager()
{
	Get<Zombie>().reserve(150);
	Get<Particle>().reserve(150);
}

EntityManager::~EntityManager()
{

}

void EntityManager::Update()
{
	UpdateStorage(Get<Zombie>());
	UpdateStorage(Get<Particle>());
}

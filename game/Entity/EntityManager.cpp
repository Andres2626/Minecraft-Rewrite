#include "Entity/EntityManager.h"

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{

}

void EntityManager::Update()
{
    auto it = m_Entities.begin();
    while (it != m_Entities.end())
    {
        (*it)->Update();

        if ((*it)->attr.isDead)
            it = m_Entities.erase(it); /* delete entity from manager */
        else
            ++it;
    }
}

void EntityManager::Register(std::unique_ptr<Entity> ent)
{
	m_Entities.push_back(std::move(ent));
}

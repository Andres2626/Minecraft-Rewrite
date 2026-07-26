#include "Model/ModelManager.h"

#include "Model/BushModel.h"
#include "Model/ParticleModel.h"
#include "Model/ZombieModel.h"

std::vector<std::unique_ptr<IModel>> ModelManager::m_Models;

void ModelManager::Init()
{
	m_Models.resize((u32t)ModelType::END);
	Register(ModelType::ZOMBIE, std::make_unique<ZombieModel>());
	Register(ModelType::PARTICLE, std::make_unique<ParticleModel>());
	Register(ModelType::BUSH, std::make_unique<BushModel>());
}

void ModelManager::Register(ModelType type, std::unique_ptr<IModel> model)
{
	m_Models[(u32t)type] = std::move(model);
}

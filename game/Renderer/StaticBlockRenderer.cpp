#include "Renderer/StaticModelRenderer.h"

#include "Model/ModelManager.h"

#include <Graphics/Renderer.h>
#include <Graphics/Shader/ShaderManager.h>
#include <Utils/Profiler.h>

StaticModelRenderer::StaticModelRenderer()
{

}

StaticModelRenderer::~StaticModelRenderer()
{

}

void StaticModelRenderer::Begin()
{
	for (auto& instances : m_Instances) {
		instances.second.clear();
	}
}

void StaticModelRenderer::Sumbit(ModelType type, std::vector<StaticBlockInstance> &instances)
{
	if (instances.size() == 0)
		return; 

	auto& batch = m_Instances[type];
	batch.insert(batch.end(), instances.begin(), instances.end());
}

void StaticModelRenderer::Sumbit(ModelType type, StaticBlockInstance &instance)
{
	m_Instances[type].push_back(instance);
}

void StaticModelRenderer::End()
{
	PROFILE_FUNCTION();

	for (auto& instance : m_Instances) {
		const ModelType &mt = instance.first;
		if ((u32t)mt > (u32t)ModelType::END) {
			mc_error("invalid instanced model type. model: {} not exist", (u32t)mt);
			return;
		}

		IInstancedModel &model = static_cast<IInstancedModel&>(ModelManager::GetModelType(mt));

		if (instance.second.empty())
			continue;

		model.GetInstancedMesh().UpdateInstances(instance.second.size(), instance.second.data());
		model.GetInstancedMesh().Render();
	}
}

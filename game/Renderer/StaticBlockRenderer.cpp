#include "Renderer/StaticModelRenderer.h"

#include "Model/ModelManager.h"

#include <Graphics/Renderer.h>
#include <Graphics/Shader/ShaderManager.h>

StaticModelRenderer::StaticModelRenderer()
{

}

StaticModelRenderer::~StaticModelRenderer()
{
}

void StaticModelRenderer::Begin()
{
	for (auto& instances : m_Instances)
		instances.second.clear();
}

void StaticModelRenderer::Sumbit(ModelType type, std::vector<StaticBlockInstance>& instances)
{
	auto& batch = m_Instances[type];
	batch.insert(batch.end(), instances.begin(), instances.end());
}

void StaticModelRenderer::Sumbit(ModelType type, StaticBlockInstance& instance)
{
	m_Instances[type].push_back(instance);
}

void StaticModelRenderer::End()
{
	for (auto& instance : m_Instances) {
		const ModelType &mt = instance.first;
		IModel &model = ModelManager::GetModelType(mt);

		if (instance.second.empty())
			continue;

		model.GetInstanceBuffer().Bind();
		model.GetInstanceBuffer().Update(0, instance.second.size() * sizeof(StaticBlockInstance), instance.second.data());

		model.Bind();
		Renderer::DrawElementsInstanced(GL_TRIANGLES, model.GetIndexCount(), nullptr, instance.second.size());
		model.Unbind();
	}
}

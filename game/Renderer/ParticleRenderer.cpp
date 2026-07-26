#include "Renderer/ParticleRenderer.h"

#include "Model/ModelManager.h"
#include "Entity/EntityManager.h"

ParticleRenderer::ParticleRenderer()
{
    m_Instances.reserve(1000);
	m_Shader = &ShaderManager::Get("particle");
}

ParticleRenderer::~ParticleRenderer()
{
}

void ParticleRenderer::Render(Player &player, float alpha)
{
    m_Shader->SetVec3("s_color", { 0.8f, 0.8f, 0.8f });
    m_Shader->SetVec3("s_camUp", player.Cam.up);
    m_Shader->SetVec3("s_camRight", player.Cam.right);

    std::vector<ParticleInstance>().swap(m_Instances);

    for (auto& z : m_Entities->Get<Particle>()) {
        vec3 p1 = mix(z->attr.oldPos, z->attr.pos, alpha);
        m_Instances.push_back({ p1, z->m_Tex, z->m_TextureOff });
    }

    if (m_Instances.empty())
        return;

    ParticleModel& model = static_cast<ParticleModel&>(ModelManager::GetModelType(ModelType::PARTICLE));

    model.GetInstanceBuffer().Bind();
    model.GetInstanceBuffer().Update(0, m_Instances.size() * sizeof(ParticleInstance), m_Instances.data());

    model.Bind();
    Renderer::DrawElementsInstanced(GL_TRIANGLES, 6, nullptr, m_Instances.size());
    model.Unbind();
}

void ParticleRenderer::SetEntityManager(EntityManager* em)
{
    m_Entities = em;
}


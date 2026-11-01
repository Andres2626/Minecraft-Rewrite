#include "Renderer/ParticleRenderer.h"

#include "Model/ModelManager.h"
#include "Entity/EntityManager.h"

ParticleRenderer::ParticleRenderer()
{
    m_Instances.reserve(1000);
	m_Shader = &ShaderManager::Get("particle");
    m_Shader->Bind();
    m_Shader->SetUniform("s_t1", 0);
    m_Shader->SetUniform("s_color", vec3(0.8f, 0.8f, 0.8f));
}

ParticleRenderer::~ParticleRenderer()
{
}

void ParticleRenderer::Render(Player &player, float alpha)
{
    PROFILE_FUNCTION();
    m_Shader->Bind();
    m_Shader->SetUniform("s_VP", player.GetVP());
    m_Shader->SetUniform("s_camUp", player.Cam.up);
    m_Shader->SetUniform("s_camRight", player.Cam.right);

    m_Instances.clear();

    for (auto& z : m_Entities->Get<Particle>()) {
        vec3 p1 = mix(z->attr.oldPos, z->attr.pos, alpha);
        m_Instances.push_back({ p1, z->m_Tex, z->m_TextureOff });
    }

    if (m_Instances.empty())
        return;

    ParticleModel &model = static_cast<ParticleModel&>(ModelManager::GetModelType(ModelType::PARTICLE));
    model.GetInstancedMesh().UpdateInstances(m_Instances.size(), m_Instances.data());
    model.GetInstancedMesh().Render();
}

void ParticleRenderer::SetEntityManager(EntityManager* em)
{
    m_Entities = em;
}


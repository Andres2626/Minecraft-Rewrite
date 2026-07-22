#include "Particle/ParticleEngine.h"

ParticleEngine::ParticleEngine()
{
    m_Particles.reserve(1000);
    m_Instances.reserve(1000);
	m_Shader = &ShaderManager::Get("particle");
}

ParticleEngine::~ParticleEngine()
{
}

void ParticleEngine::Add(std::unique_ptr<Particle> particle)
{
	m_Particles.push_back(std::move(particle));
}

void ParticleEngine::Update()
{
    auto it = m_Particles.begin();
    while (it != m_Particles.end())
    {
        (*it)->Update();

        if ((*it)->attr.isDead)
            it = m_Particles.erase(it); /* delete particle */
        else
            ++it;
    }
}

void ParticleEngine::Render(Player &player, float alpha)
{
    m_Shader->SetVec3("s_color", { 0.8f, 0.8f, 0.8f });
    m_Shader->SetVec3("s_camUp", player.Cam.up);
    m_Shader->SetVec3("s_camRight", player.Cam.right);

    std::vector<ParticleInstance>().swap(m_Instances);

    for (auto& z : m_Particles) {
        vec3 p1 = mix(z->attr.oldPos, z->attr.pos, alpha);
        m_Instances.push_back({ p1, z->m_Tex, z->m_TextureOff });
    }

    if (m_Instances.empty())
        return;

    ParticleModel::Get().UpdateInstances(m_Instances);
    ParticleModel::Get().Bind();

    Renderer::DrawElementsInstanced(GL_TRIANGLES, 6, nullptr, m_Instances.size());

    ParticleModel::Get().Unbind();
}

#include "Character/ZombieRenderer.h"

#include "Entity/EntityManager.h"
#include "Level/Level.h"
#include "GameProperties.h"

ZombieRenderer::ZombieRenderer()
{
	m_Shader = &ShaderManager::Get("character");
}

ZombieRenderer::~ZombieRenderer()
{
}

void ZombieRenderer::Render(Level &lev, Player &player, float alpha, float seconds)
{
	/*
	 * Actually, this shouldn't go here.
	 * TODO: Create the Render3D, Scene3D and RenderCommand classes.
	 */
	m_Shader->SetVec3("s_cpos", player.Cam.pos);
	m_Shader->SetVec4("s_fcolor0", GlobalGP.fg0.color);
	m_Shader->SetFloat("s_fdensity0", GlobalGP.fg0.density);
	m_Shader->SetVec4("s_fcolor1", GlobalGP.fg1.color);
	m_Shader->SetFloat("s_fdensity1", GlobalGP.fg1.density);
	m_Shader->SetFloat("s_seconds", fmod(seconds, 2 * M_PI));
	m_Shader->SetFloat("s_scale", 0.058333334f);

	std::vector<ZombieInstance>().swap(m_Instances);

	for (auto& z : m_Entities->Get<Zombie>()) {
		if (player.Cam.InFrustum(z->attr.box)) {
			vec3 p1 = mix(z->attr.oldPos, z->attr.pos, alpha);
			m_Instances.push_back({ p1, z->rot + 180.0f, z->timeoff, z->attr.speed, lev.GetBrigthness(z->attr.pos) });
		}
	}

	if (m_Instances.empty())
		return;

	ZombieModel::Get().UpdateInstances(m_Instances);
	ZombieModel::Get().Bind();

	const auto& parts = ZombieModel::Get().GetParts();

	for (size_t i = 0; i < parts.size(); i++) {
		m_Shader->SetInt("s_partindex", (int)i);
		m_Shader->SetVec3("s_partrot", parts[i].rot);

		Renderer::DrawElementsInstanced(GL_TRIANGLES, parts[i].size, (void*)(parts[i].offset * sizeof(u32t)), (GLsizei)m_Instances.size());
	}

	ZombieModel::Get().Unbind();
}

void ZombieRenderer::SetEntityManager(EntityManager* em)
{
	m_Entities = em;
}

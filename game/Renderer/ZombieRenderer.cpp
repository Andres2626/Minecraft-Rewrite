#include "Renderer/ZombieRenderer.h"

#include "Model/ModelManager.h"
#include "Entity/EntityManager.h"
#include "Level/Level.h"
#include "GameProperties.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <Utils/Profiler.h>

ZombieRenderer::ZombieRenderer()
{
	m_Shader = &ShaderManager::Get("character");
	m_Shader->Bind();
	m_Shader->SetUniform("s_t1", 0);
	m_Shader->SetUniform("s_fcolor0", GlobalGP.fg0.color);
	m_Shader->SetUniform("s_fdensity0", GlobalGP.fg0.density);
	m_Shader->SetUniform("s_fcolor1", GlobalGP.fg1.color);
	m_Shader->SetUniform("s_fdensity1", GlobalGP.fg1.density);
	m_Shader->SetUniform("s_scale", 0.058333334f);
}

ZombieRenderer::~ZombieRenderer()
{

}

void ZombieRenderer::Render(Level &lev, Texture& tex, Player &player, float alpha, float seconds)
{
	PROFILE_FUNCTION();
	/*
	 * Actually, this shouldn't go here.
	 * TODO: Create the Render3D, Scene3D and RenderCommand classes.
	 */
	m_Shader->Bind();
	m_Shader->SetUniform("s_VP", player.GetVP());
	m_Shader->SetUniform("s_cpos", player.Cam.pos);
	m_Shader->SetUniform("s_seconds", (float)fmod(seconds, 2 * M_PI));
	
	m_Instances.clear();

	for (auto& z : m_Entities->Get<Zombie>()) {
		if (player.Cam.InFrustum(z->attr.box)) {
			vec3 p1 = mix(z->attr.oldPos, z->attr.pos, alpha);
			m_Instances.push_back({ p1, z->rot + 180.0f, z->timeoff, z->attr.speed, lev.GetBrigthness(z->attr.pos) });
		}
	}

	if (m_Instances.empty())
		return;

	ZombieModel& model = static_cast<ZombieModel&>(ModelManager::GetModelType(ModelType::ZOMBIE));

	model.GetInstancedMesh().UpdateInstances(m_Instances.size(), m_Instances.data());
	model.Bind();

	tex.Bind(0);

	const auto& parts = model.GetParts();

	for (size_t i = 0; i < parts.size(); i++) {
		m_Shader->SetUniform("s_partindex", (int)i);
		m_Shader->SetUniform("s_partrot", parts[i].rot);

		Renderer::DrawElementsInstanced(GL_TRIANGLES, parts[i].size, (void*)(parts[i].offset * sizeof(u32t)), (GLsizei)m_Instances.size());
	}

	model.Unbind();
}

void ZombieRenderer::SetEntityManager(EntityManager* em)
{
	m_Entities = em;
}

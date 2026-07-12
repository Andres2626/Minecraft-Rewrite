#include "Entity/EntityRenderer.h"

#include "Character/Zombie.h"
#include "GameProperties.h"

EntityRenderer::EntityRenderer()
	: m_EM(NULL)
{
	m_Shader = &ShaderManager::Get("character");
}

EntityRenderer::~EntityRenderer()
{
	
}

void EntityRenderer::SetEntityManager(EntityManager* ents)
{
	m_EM = ents;
}

void EntityRenderer::Render(Level &lev, Player &player, float alpha, float seconds)
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

	ZombieModel::Get().Bind();

	for (auto &z : m_EM->GetEntities()) {
		if (player.Cam.InFrustum(z->attr.box)) {
			m_Shader->SetFloat("s_brightness", lev.GetBrigthness(z->attr.pos));
			z->Render(alpha, seconds);
		}
	}

	ZombieModel::Get().Unbind();
}

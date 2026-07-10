#include "Entity/EntityRenderer.h"

EntityRenderer::EntityRenderer()
	: m_EM(NULL)
{

}

EntityRenderer::~EntityRenderer()
{

}

void EntityRenderer::SetEntityManager(EntityManager* ents)
{
	m_EM = ents;
}

void EntityRenderer::Render(Level &lev, Camera &cam, Shader* shader, float alpha, float seconds)
{
	for (auto &z : m_EM->GetEntities()) {
		if (cam.InFrustum(z->attr.box)) {
			shader->SetFloat("s_brightness", lev.GetBrigthness(z->attr.pos));
			z->Render(shader, alpha, seconds);
		}
	}
}

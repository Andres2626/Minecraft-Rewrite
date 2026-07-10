#include "Player/Selector.h"

Selector::Selector() 
{
	m_Mesh = std::make_unique<Mesh>(&m_MeshData);

	int count = (int)m_MeshData.vertices.size();
	m_MeshData.vertices.push_back({ { 0.0f, 0.0f, 0.0f } });
	m_MeshData.vertices.push_back({ { 1.0f, 0.0f, 0.0f } });
	m_MeshData.vertices.push_back({ { 1.0f, 0.0f, 1.0f } });
	m_MeshData.vertices.push_back({ { 0.0f, 0.0f, 1.0f } });
	MeshFactory::AddIndices(m_MeshData, count);

	m_Mesh->Build();
}

Selector::~Selector() 
{
	
}

void Selector::SetHit(const Hitresult& hit) 
{
	m_Hit = hit;
}

void Selector::Render(Camera& cam, Shader* shader, float millis) 
{
	mat4 model = mat4(1.0f);
	mat4 rot = mat4(1.0f);

	/*
	 * separate the vertices of the block selector a little to avoid
	 * z-fighting
	 */
	const float s = 0.002f;


	/*
	 * To avoid adding the block position directly to the selector 
	 * buffer, the model matrix is used instead to rotate and translate 
	 * the selector to the block and the desired face.
	 */
	if (this->m_Hit.face == ivec3(0, 0, -1)) {
		rot = rotate(rot, radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
		model = translate(model, vec3(m_Hit.block) - s) * rot;
	}
	else if (this->m_Hit.face == ivec3(0, 0, 1)) {
		rot = rotate(rot, radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
		model = translate(model, vec3(m_Hit.block) + vec3(m_Hit.face) + s) * rot;
	}
	else if (this->m_Hit.face == ivec3(-1, 0, 0)) {
		rot = rotate(rot, radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
		model = translate(model, vec3(m_Hit.block) - s) * rot;
	}
	else if (this->m_Hit.face == ivec3(1, 0, 0)) {
		rot = rotate(rot, radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
		model = translate(model, vec3(m_Hit.block) + vec3(m_Hit.face) + s) * rot;
	}
	else if (this->m_Hit.face == ivec3(0, 1, 0)) {
		model = translate(model, vec3(m_Hit.block) + vec3(m_Hit.face) + s) * rot;
	}
	else {
		model = translate(model, vec3(m_Hit.block) - s) * rot;
	}

	shader->Set4x4("s_M", model);
	shader->SetFloat("s_Timer", millis);

	Renderer::Enable(GL_BLEND);
	Renderer::BlendFunc(BlendValue::SRC_ALPHA, BlendValue::ONE_MINUS_SRC_ALPHA);

	m_Mesh->Render();

	Renderer::Disable(GL_BLEND);
}
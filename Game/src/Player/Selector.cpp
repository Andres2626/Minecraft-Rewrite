#include "Selector.h"

Selector::Selector() {
	/* Predefined selector face vertices and indices */
	float vertices[12] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};

	unsigned int indices[6] = {
		0, 1, 2,
		2, 3, 0
	};
	
	/* BUILD MESH PROCESS
	 *  -- Create vertex layout object
	 *  -- Reset buffer pointer (VAO, IBO and VBO)
	 *  -- Push all vertices/indices in buffer array
	 *  -- Setup mesh attributes
	 *  -- Link attributes to VAO
	 *  -- Unbind VAO
	 */
	MC::Graphics::VertexLayout VL;
	this->VAO = std::make_unique<MC::Graphics::VertexArray>();
	this->VBO = std::make_unique <MC::Graphics::VertexBuffer>();
	this->IBO = std::make_unique <MC::Graphics::IndexBuffer>();
	this->VBO->Build(12 * sizeof(float), vertices);
	this->IBO->Build(6 * sizeof(unsigned int), indices);
	VL.AddAttribute(SHADER_VERTEX_BIT, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	this->VAO->Link(VL);
	this->VAO->Unbind();
}

Selector::~Selector() {
	/* delete selector mesh */
	this->VAO.reset();
	this->VBO.reset();
	this->IBO.reset();
}

void Selector::SetHit(const Hitresult& hit) {
	/* set hit info */
	this->m_Hit = hit;
}

void Selector::Render(MC::Graphics::Camera& cam, MC::Graphics::Shader* shader, float millis) {
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);

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
	if (this->m_Hit.face == glm::ivec3(0, 0, -1)) {
		rot = glm::rotate(rot, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(this->m_Hit.block) - s) * rot;
	}
	else if (this->m_Hit.face == glm::ivec3(0, 0, 1)) {
		rot = glm::rotate(rot, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(this->m_Hit.block) + glm::vec3(m_Hit.face) + s) * rot;
	}
	else if (this->m_Hit.face == glm::ivec3(-1, 0, 0)) {
		rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(this->m_Hit.block) - s) * rot;
	}
	else if (this->m_Hit.face == glm::ivec3(1, 0, 0)) {
		rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(this->m_Hit.block) + glm::vec3(m_Hit.face) + s) * rot;
	}
	else if (this->m_Hit.face == glm::ivec3(0, 1, 0)) {
		model = glm::translate(model, glm::vec3(this->m_Hit.block) + glm::vec3(m_Hit.face) + s) * rot;
	}
	else {
		model = glm::translate(model, glm::vec3(this->m_Hit.block) - s) * rot;
	}

	/* Enable blending for selector */
	MC::Graphics::Renderer::Enable(GL_BLEND);
	MC::Graphics::Renderer::BlendFunc(MC::Graphics::BlendValue::SRC_ALPHA, MC::Graphics::BlendValue::ONE_MINUS_SRC_ALPHA);

	/* Translate and rotate selector */
	shader->Set4x4("s_M", model);
	shader->SetFloat("s_Timer", millis); /* obtained by timer->ElapsedMillis() */

	/* SELECTOR DRAW PROCESS */
	this->VAO->Bind();
	MC::Graphics::Renderer::DrawElements(GL_TRIANGLES, IBO->GetSize());
	this->VAO->Unbind();

	/* Disable blending */
	MC::Graphics::Renderer::Disable(GL_BLEND);
}

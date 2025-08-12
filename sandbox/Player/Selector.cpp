#include "Player/Selector.h"

Selector::Selector() 
{
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
	VertexLayout VL;
	this->VAO = std::make_unique<VertexArray>();
	this->VBO = std::make_unique <VertexBuffer>();
	this->IBO = std::make_unique <IndexBuffer>();
	this->VBO->Build(12 * sizeof(float), vertices);
	this->IBO->Build(6 * sizeof(unsigned int), indices);
	VL.AddAttribute(SHADER_VERTEX_BIT, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	this->VAO->Link(VL);
	this->VAO->Unbind();
}

Selector::~Selector() 
{
	/* delete selector mesh */
	this->VAO.reset();
	this->VBO.reset();
	this->IBO.reset();
}

void Selector::SetHit(const Hitresult& hit) 
{
	/* set hit info */
	this->m_Hit = hit;
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
		model = translate(model, vec3(this->m_Hit.block) - s) * rot;
	}
	else if (this->m_Hit.face == ivec3(0, 0, 1)) {
		rot = rotate(rot, radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
		model = translate(model, vec3(this->m_Hit.block) + vec3(m_Hit.face) + s) * rot;
	}
	else if (this->m_Hit.face == ivec3(-1, 0, 0)) {
		rot = rotate(rot, radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
		model = translate(model, vec3(this->m_Hit.block) - s) * rot;
	}
	else if (this->m_Hit.face == ivec3(1, 0, 0)) {
		rot = rotate(rot, radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
		model = translate(model, vec3(this->m_Hit.block) + vec3(m_Hit.face) + s) * rot;
	}
	else if (this->m_Hit.face == ivec3(0, 1, 0)) {
		model = translate(model, vec3(this->m_Hit.block) + vec3(m_Hit.face) + s) * rot;
	}
	else {
		model = translate(model, vec3(this->m_Hit.block) - s) * rot;
	}

	/* Enable blending for selector */
	Renderer::Enable(GL_BLEND);
	Renderer::BlendFunc(BlendValue::SRC_ALPHA, BlendValue::ONE_MINUS_SRC_ALPHA);

	/* Translate and rotate selector */
	shader->Set4x4("s_M", model);
	shader->SetFloat("s_Timer", millis); /* obtained by timer->ElapsedMillis() */

	/* SELECTOR DRAW PROCESS */
	this->RenderInternal();

	/* Disable blending */
	Renderer::Disable(GL_BLEND);
}

void Selector::RenderInternal() 
{
	this->VAO->Bind();
	Renderer::DrawElements(GL_TRIANGLES, IBO->GetSize());
	this->VAO->Unbind();
}
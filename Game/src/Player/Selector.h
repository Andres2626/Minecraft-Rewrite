#pragma once
#include "Hitresult.h"

#include <Graphics/Shader/Shader.h>
#include <Graphics/Shader/Shaderflags.h>
#include <Graphics/Buffers/VertexArray.h>
#include <Graphics/Buffers/VertexBuffer.h>
#include <Graphics/Buffers/IndexBuffer.h>
#include <Graphics/Buffers/VertexLayout.h>
#include <Graphics/Renderer.h>
#include <Graphics/Camera/Camera.h>

class Selector {
private:
	std::unique_ptr<MC::Graphics::VertexArray> VAO;
	std::unique_ptr<MC::Graphics::VertexBuffer> VBO;
	std::unique_ptr<MC::Graphics::IndexBuffer> IBO;
	Hitresult m_Hit;
public:
	Selector();
	~Selector();
public:
	void SetHit(const Hitresult& hit);
	void Render(MC::Graphics::Camera& cam, MC::Graphics::Shader* shader, float millis);
};
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

using namespace MC;
using namespace Graphics;
using namespace Math;

class Selector {
private:
	std::unique_ptr<VertexArray> VAO;
	std::unique_ptr<VertexBuffer> VBO;
	std::unique_ptr<IndexBuffer> IBO;
	Hitresult m_Hit;
public:
	Selector();
	~Selector();
public:
	void SetHit(const Hitresult& hit);
	void Render(Camera& cam, Shader* shader, float millis);
	void RenderInternal();
};
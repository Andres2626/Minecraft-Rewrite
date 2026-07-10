#pragma once
#include "Hitresult.h"

#include <Graphics/Shader/Shader.h>
#include <Graphics/Shader/Shaderflags.h>
#include <Graphics/MeshFactory.h>
#include <Graphics/Renderer.h>
#include <Graphics/Camera/Camera.h>

using namespace MC;
using namespace Graphics;
using namespace Math;

class Selector 
{
private:
	MeshData m_MeshData;
	std::unique_ptr<Mesh> m_Mesh;
	Hitresult m_Hit;
public:
	Selector();
	~Selector();
public:
	void SetHit(const Hitresult& hit);
	void Render(Camera& cam, Shader* shader, float millis);
};
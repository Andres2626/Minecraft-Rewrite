#pragma once

#include "Block/Block.h"
#include "Block/Types/Grass.h"
#include "Player/Player.h"

#include <Math/Math.h>
#include <Graphics/Buffers/IndexBuffer.h>
#include <Graphics/Renderer.h>
#include <Graphics/Renderer2D.h>
#include <Graphics/Camera/OrthographicCamera.h>
#include <Graphics/Shader/Shader.h>
#include <Graphics/MeshFactory.h>
#include <Graphics/Texture.h>

using namespace MC;
using namespace Graphics;
using namespace Math;

class gui
{
protected:
	Player *m_Player;
	Renderer2D m_Renderer;
	std::unique_ptr<Mesh> m_Mesh;
	MeshData m_MeshData;
private:
	vec2 m_WinSize;
	std::unique_ptr<OrthographicCamera> m_Cam;
public:
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
public:
	gui(Player *player, const vec2 &winsize);
	~gui();
public:
	void Build();
	void BuildCrossHair();
	void Render(Shader *shader, Texture *tex);
public:
	void OnWindowResize(const vec2 &winsize);
public:
	void AddFace(const ivec3 &fpos, Face f, Block &t);
	void AddQuad(const vec2 &uv, const ivec3 &pos, const ivec3 *quad);
};
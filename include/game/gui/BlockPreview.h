#pragma once

#include "Block/BlockManager.h"
#include "Model/BlockModel.h"
#include "Model/ModelManager.h"
#include "Player/Player.h"

#include <Graphics/Camera/OrthographicCamera.h>
#include <Graphics/MeshFactory.h>
#include <Graphics/Shader/ShaderManager.h>
#include <Graphics/Texture.h>
#include <Math/Math.h>

#include <memory>

using namespace MC;
using namespace Graphics;
using namespace Math;

class BlockPreview
{
private:
	Shader *m_BPShader;
	Block *m_SelectedBlock;
	std::unique_ptr<Mesh> m_Mesh;
public:
	BlockPreview();
	~BlockPreview();
public:
	void Build(Player *player);
	void Render(vec2 winsize, Texture *tex, OrthographicCamera *cam);
private:
	void AddFace(MeshData &buff, Face f, Block &t);
public:
	Block *GetSelectedBlock() { return m_SelectedBlock; }
};
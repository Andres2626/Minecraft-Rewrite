#pragma once

#include "BlockPreview.h"

#include "Block/Block.h"
#include "Block/Types/Grass.h"
#include "Player/Player.h"
#include "Font/DefaultFont.h"

#include <Math/Math.h>
#include <Graphics/Renderer.h>
#include <Graphics/Renderer2D.h>
#include <Graphics/Camera/OrthographicCamera.h>
#include <Graphics/Shader/ShaderManager.h>
#include <Graphics/MeshFactory.h>
#include <Graphics/Texture.h>

#include <memory>

using namespace MC;
using namespace Graphics;
using namespace Math;

class gui
{
protected:
	int m_FPS;
	int m_CUPS;
	Player *m_Player;
	Renderer2D m_Renderer;
	Shader *m_Shader;
	BlockPreview m_BP;
	std::unique_ptr<DefaultFont> m_Font;
private:
	vec2 m_GUISize;
	std::unique_ptr<OrthographicCamera> m_Cam;
public:
	gui(Player *player, const vec2 &winsize);
	~gui();
public:
	void Build();
	void BuildCrossHair();
	void RenderSelectedBlock(Texture *tex);
	void Render(Texture *tex);
public:
	void OnTick(int fps, int cups);
};
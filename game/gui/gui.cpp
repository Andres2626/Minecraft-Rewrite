#include "gui/gui.h"

#include "Block/BlockManager.h"

#include <array>

static std::array<ivec3, 4> front = {
	ivec3(0, 0, 0),
	ivec3(1, 0, 0),
	ivec3(1, 1, 0),
	ivec3(0, 1, 0)
};

static std::array<ivec3, 4> back = {
	ivec3(1, 0, 1),
	ivec3(0, 0, 1),
	ivec3(0, 1, 1),
	ivec3(1, 1, 1)
};

static std::array<ivec3, 4> left = {
	ivec3(0, 0, 0),
	ivec3(0, 0, 1),
	ivec3(0, 1, 1),
	ivec3(0, 1, 0)
};

static std::array<ivec3, 4> right = {
	ivec3(1, 0, 1),
	ivec3(1, 0, 0),
	ivec3(1, 1, 0),
	ivec3(1, 1, 1)
};

static std::array<ivec3, 4> bottom = {
	ivec3(0, 0, 1),
	ivec3(1, 0, 1),
	ivec3(1, 0, 0),
	ivec3(0, 0, 0)
};

static std::array<ivec3, 4> top = {
	ivec3(0, 1, 0),
	ivec3(1, 1, 0),
	ivec3(1, 1, 1),
	ivec3(0, 1, 1)
};


gui::gui(Player *player, const vec2 &winsize)
{
	m_Player = player;
	m_WinSize = winsize;
	m_Cam = std::make_unique<OrthographicCamera>(0.0f, m_WinSize.x, 0.0f, m_WinSize.y, 100.0f, 300.0f);
	m_Shader = &ShaderManager::Get("hud");
}

gui::~gui()
{

}

void gui::Build()
{
	m_Mesh = std::make_unique<Mesh>(&m_MeshData);

	Block blk = BlockManager::GetBlockType(m_Player->GetSelectedBlock());
	for (int i = 0; i < 6; i++) {
		AddFace({ -2.0f, 0.0f, 0.0f }, (Face)i, blk);
	}

	m_Mesh->Build();
}

void gui::BuildCrossHair()
{
	float x = m_WinSize.x / 2;
	float y = m_WinSize.y / 2;

	m_Renderer.Begin();
	m_Renderer.FillRect({ x - 8.0f, y + 0.1f }, { 17.0f, 1.0f }, { 1.0f, 1.0f, 1.0f });
	m_Renderer.FillRect({ x, y - 8.0f }, { 1.0f, 17.0f }, { 1.0f, 1.0f, 1.0f });
	m_Renderer.End();
}

void gui::Render(Texture *tex)
{
	m_Cam->Update();

	Renderer::Clear(GL_DEPTH_BUFFER_BIT);

	m_Shader->Bind();
	glm::mat4 VM = glm::mat4(1.0f);
	VM = translate(VM, { 0.0f, 0.0f, -200.0f });
	m_Shader->Set4x4("s_VP", m_Cam->GetProjection() * VM);
	m_Shader->Set4x4("s_M", glm::mat4(1.0f));

	m_Shader->SetInt("s_RenderFlags", SHADER_COLOR_FLAG);
	Renderer::Disable(GL_DEPTH_TEST);
	m_Renderer.Render();
	Renderer::Enable(GL_DEPTH_TEST);
	
	mat4 TileModel = glm::mat4(1.0f);
	TileModel = translate(TileModel, { m_WinSize.x - 48.0f, m_WinSize.y - 48.0f, 0.0f });
	TileModel = scale(TileModel, { 48.0f, 48.0f, 48.0f });
	TileModel = rotate(TileModel, glm::radians(30.0f), { 1.0F, 0.0F, 0.0F });
	TileModel = rotate(TileModel, glm::radians(45.0f), { 0.0F, 1.0F, 0.0F });
	TileModel = translate(TileModel, { 1.5F, -0.5F, -0.5F });
	m_Shader->Set4x4("s_M", TileModel);

	/* render tile */
	tex->Bind(0);
	m_Shader->SetInt("s_RenderFlags", SHADER_TEXTURE_FLAG);
	m_Mesh->Render();
}

void gui::OnWindowResize(const vec2 &winsize)
{
	m_WinSize = winsize;
	m_Cam->zright = winsize.x;
	m_Cam->ztop = winsize.y;
	BuildCrossHair();
}

void gui::AddFace(const ivec3& fpos, Face f, Block& t)
{
	int count = (int)m_MeshData.vertices.size();

	/* brigthness face values */
	float c1 = 1.0f;
	float c2 = 0.8f;
	float c3 = 0.6f;
	vec2 uv = { 0.0f, 0.0f };
	ivec3* quad;

	switch (f)
	{
	case Face::FRONT:
		quad = front.data();
		uv = t.front;
		break;
	case Face::BACK:
		quad = back.data();
		uv = t.back;
		break;
	case Face::LEFT:
		quad = left.data();
		uv = t.left;
		break;
	case Face::RIGHT:
		quad = right.data();
		uv = t.right;
		break;
	case Face::BOTTOM:
		quad = bottom.data();
		uv = t.bottom;
		break;
	case Face::TOP:
		quad = top.data();
		uv = t.top;
		break;
	default:
		return;
	}

	AddQuad(uv, fpos, quad);

	MeshFactory::AddIndices(m_MeshData, count);
}

void gui::AddQuad(const vec2& uv, const ivec3& pos, const ivec3* quad)
{
	vec2 uvs[] = {
		uv + vec2(0, UV_COORD),
		uv + vec2(UV_COORD, UV_COORD),
		uv + vec2(UV_COORD, 0),
		uv
	};

	vec3 color = vec3(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < 4; i++) {
		m_MeshData.vertices.push_back({ quad[i] + pos, color, uvs[i], 1.0f });
	}
}
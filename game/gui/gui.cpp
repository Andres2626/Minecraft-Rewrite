#include "gui/gui.h"

#include "Block/BlockManager.h"

#include "Model/ModelManager.h"
#include "Model/BushModel.h"

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
	m_GUISize.x = winsize.x * 240 / winsize.y;
	m_GUISize.y = winsize.y * 240 / winsize.y;
	m_Cam = std::make_unique<OrthographicCamera>(0.0f, m_GUISize.x, 0.0f, m_GUISize.y, 100.0f, 300.0f);
	m_Mesh = std::make_unique<Mesh>(&m_MeshData);
	m_Shader = &ShaderManager::Get("hud");
}

gui::~gui()
{

}

void gui::Build()
{
	m_SelectedBlock = &BlockManager::GetBlockType(m_Player->GetSelectedBlock());
	if (m_SelectedBlock->GetBlockFlags().RenderInstance)
		return; /* Use BlockModel instead of mesh */

	for (int i = 0; i < 6; i++) {
		AddFace((Face)i, *m_SelectedBlock);
	}

	m_Mesh->Bind();
	m_Mesh->Build();
	m_Mesh->Unbind();
}

void gui::BuildCrossHair()
{
	float x = m_GUISize.x / 2;
	float y = m_GUISize.y / 2;

	m_Renderer.Begin();
	m_Renderer.FillRect({ x, y - 4.0f }, { 1.0f, 9.0f }, { 1.0f, 1.0f, 1.0f });
	m_Renderer.FillRect({ x - 4.0f, y }, { 9.0f, 1.0f }, { 1.0f, 1.0f, 1.0f });
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
	TileModel = translate(TileModel, { m_GUISize.x - 16.0f, m_GUISize.y - 16.0f, 0.0f });
	TileModel = scale(TileModel, { 16.0f, 16.0f, 16.0f });
	TileModel = rotate(TileModel, glm::radians(30.0f), { 1.0F, 0.0F, 0.0F });
	TileModel = rotate(TileModel, glm::radians(45.0f), { 0.0F, 1.0F, 0.0F });
	TileModel = translate(TileModel, { 1.5F, -0.5F, -0.5F });
	m_Shader->Set4x4("s_M", TileModel);

	/* render tile */
	tex->Bind(0);
	m_Shader->SetVec3("s_BlockPos", { -2.0f, 0.0f, 0.0f });
	m_Shader->SetInt("s_RenderFlags", SHADER_TEXTURE_FLAG);

	Renderer::Enable(GL_BLEND);

	if (m_SelectedBlock->GetBlockFlags().RenderInstance) {
		BushModel &model = static_cast<BushModel&>(ModelManager::GetModelType(m_SelectedBlock->GetModelType()));
		model.Bind();
		Renderer::DrawElements(GL_TRIANGLES, model.GetIndexCount());
		model.Unbind();
	}
	else
		m_Mesh->Render();

	Renderer::Disable(GL_BLEND);
}

void gui::OnWindowResize(const vec2 &winsize)
{
	m_GUISize.x = winsize.x * 240 / winsize.y;
	m_GUISize.y = winsize.y * 240 / winsize.y;
	m_Cam->zright = winsize.x;
	m_Cam->ztop = winsize.y;
	BuildCrossHair();
}

void gui::AddFace(Face f, Block& t)
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

	AddQuad(uv, quad);

	MeshFactory::AddIndices(m_MeshData, count);
}

void gui::AddQuad(const vec2& uv, const ivec3* quad)
{
	vec2 uvs[] = {
		uv + vec2(0, UV_COORD),
		uv + vec2(UV_COORD, UV_COORD),
		uv + vec2(UV_COORD, 0),
		uv
	};

	vec3 color = vec3(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < 4; i++) {
		m_MeshData.vertices.push_back({ quad[i], color, uvs[i], 1.0f });
	}
}
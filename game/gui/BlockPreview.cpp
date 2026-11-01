#include "gui/BlockPreview.h"

#include <Utils/Util.h>

BlockPreview::BlockPreview()
{
	VertexLayout VL;
	VL.AddAttribute<Math::vec3>(SHADER_VERTEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	VL.AddAttribute<Math::vec3>(SHADER_COLOR_BIT, sizeof(Vertex), (void*)offsetof(Vertex, color));
	VL.AddAttribute<Math::vec2>(SHADER_TEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	m_Mesh = std::make_unique<Mesh>(VL);
	m_BPShader = &ShaderManager::Get("hud2");
}

BlockPreview::~BlockPreview()
{
}

void BlockPreview::Build(Player *player)
{
	if (!player) {
		mc_fatal("invalid player pointer");
		return;
	}

	MeshData buff;

	m_SelectedBlock = &BlockManager::GetBlockType(player->GetSelectedBlock());
	if (m_SelectedBlock->GetBlockFlags().RenderInstance)
		return; /* Use BlockModel instead of mesh */

	for (int i = 0; i < 6; i++) {
		AddFace(buff, (Face)i, *m_SelectedBlock);
	}

	m_Mesh->Bind();
	m_Mesh->Build(&buff);
	m_Mesh->Unbind();
}

void BlockPreview::AddFace(MeshData &buff, Face f, Block& t)
{
	mc_assert(((int)f >= 0 && (int)f <= 5), "invalid block face");
	int count = (int)buff.vertices.size();

	vec2 uv = { 0.0f, 0.0f };
	BlockModel &model = static_cast<BlockModel&>(ModelManager::GetModelType(ModelType::BLOCK));
	ivec3* quad = model.GetBlockFace(f);

	switch (f)
	{
	case Face::FRONT:
		uv = t.front;
		break;
	case Face::BACK:
		uv = t.back;
		break;
	case Face::LEFT:
		uv = t.left;
		break;
	case Face::RIGHT:
		uv = t.right;
		break;
	case Face::BOTTOM:
		uv = t.bottom;
		break;
	case Face::TOP:
		uv = t.top;
		break;
	default:
		return;
	}

	vec2 uvs[] = {
		uv + vec2(0, UV_COORD),
		uv + vec2(UV_COORD, UV_COORD),
		uv + vec2(UV_COORD, 0),
		uv
	};

	Vertex v0 = { quad[0], vec3(1.0f), uvs[0] };
	Vertex v1 = { quad[1], vec3(1.0f), uvs[1] };
	Vertex v2 = { quad[2], vec3(1.0f), uvs[2] };
	Vertex v3 = { quad[3], vec3(1.0f), uvs[3] };

	MeshFactory::AddQuad(buff, v0, v1, v2, v3);
	MeshFactory::AddIndices(buff, count);
}

void BlockPreview::Render(vec2 winsize, Texture *tex, OrthographicCamera *cam)
{
	glm::mat4 VM = glm::mat4(1.0f);
	VM = translate(VM, { 0.0f, 0.0f, -200.0f });

	mat4 TileModel = glm::mat4(1.0f);
	TileModel = translate(TileModel, { winsize.x - 16.0f, winsize.y - 16.0f, 0.0f });
	TileModel = scale(TileModel, { 16.0f, 16.0f, 16.0f });
	TileModel = rotate(TileModel, glm::radians(30.0f), { 1.0f, 0.0f, 0.0f });
	TileModel = rotate(TileModel, glm::radians(45.0f), { 0.0f, 1.0f, 0.0f });
	TileModel = translate(TileModel, { 1.5f, -0.5f, -0.5f });
	TileModel = translate(TileModel, { -2.0f, 0.0f, 0.0f });

	m_BPShader->Bind();
	m_BPShader->SetUniform("s_VP", cam->GetProjection() * VM);
	m_BPShader->SetUniform("s_M", TileModel);

	/* render tile */
	tex->Bind(0);

	if (m_SelectedBlock->GetBlockFlags().RenderInstance) {
		IInstancedModel& model = static_cast<IInstancedModel&>(ModelManager::GetModelType(m_SelectedBlock->GetModelType()));
		model.Bind();
		Renderer::DrawElements(GL_TRIANGLES, model.GetIndexCount());
		model.Unbind();
	}
	else
		m_Mesh->Render();
}

#include "Graphics/Renderer2D.h"

#include "Graphics/Shader/Shaderflags.h"

#include "gfx/glad.h"

#define RENDERER2D_INITIAL_VERTICES 1000
#define RENDERER2D_INITIAL_INDICES 6000

namespace MC 
{
	namespace Graphics
	{
		Renderer2D::Renderer2D()
		{
			m_VAO = std::make_unique<VertexArray>();
			m_IBO = std::make_unique<IndexBuffer>();
			m_VBO = std::make_unique<VertexBuffer>(GL_STATIC_DRAW);

			m_quadvert.reserve(RENDERER2D_INITIAL_VERTICES);
			m_quadind.reserve(RENDERER2D_INITIAL_INDICES);
		}

		Renderer2D::~Renderer2D()
		{
		}

		void Renderer2D::Begin()
		{

		}

		void Renderer2D::End()
		{
			VertexLayout VL;
			m_VAO->Bind();
			m_VBO->Build(m_quadvert.size() * sizeof(Vertex), m_quadvert.data());
			m_IBO->Build(m_quadind.size(), m_quadind.data());
			VL.AddAttribute<Math::vec3>(SHADER_VERTEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
			VL.AddAttribute<Math::vec3>(SHADER_COLOR_BIT, sizeof(Vertex), (void*)offsetof(Vertex, color));
			VL.AddAttribute<Math::vec2>(SHADER_TEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
			VL.AddAttribute<float>(SHADER_BRIG_BIT, sizeof(Vertex), (void*)offsetof(Vertex, brig));
			m_VBO->SetVertexLayout(VL);
			m_VAO->Unbind();

			std::vector<Vertex>().swap(m_quadvert);
			std::vector<uint32_t>().swap(m_quadind);
		}

		void Renderer2D::AddVertex(const Vertex& vert)
		{
			m_quadvert.push_back(vert);
		}

		void Renderer2D::AddQuad(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4)
		{
			int count = (int)m_quadvert.size();
			m_quadvert.push_back(v1);
			m_quadvert.push_back(v2);
			m_quadvert.push_back(v3);
			m_quadvert.push_back(v4);
			PushIndices(count);
		}

		void Renderer2D::DrawLine(Math::vec2 start, Math::vec2 end, Math::vec3 color, float thickness)
		{
			Math::vec2 dir = normalize(end - start);

			Math::vec2 perp(-dir.y, dir.x);
			perp *= thickness * 0.5f;

			Math::vec2 v0 = start - perp;
			Math::vec2 v1 = start + perp;
			Math::vec2 v2 = end + perp;
			Math::vec2 v3 = end - perp;

			AddQuad({ { v0.x, v0.y, 0.0f }, color, { 0.0f, 0.0f }, 1.0f }, 
					{ { v1.x, v1.y, 0.0f }, color, { 1.0f, 0.0f }, 1.0f }, 
					{ { v2.x, v2.y, 0.0f }, color, { 1.0f, 1.0f }, 1.0f }, 
					{ { v3.x, v3.y, 0.0f }, color, { 0.0f, 1.0f }, 1.0f });
		}

		void Renderer2D::DrawRect(Math::vec2 start, Math::vec2 size, Math::vec3 color, float thickness)
		{
			FillRect(start, { size.x, thickness }, color);
			FillRect({ start.x, start.y + size.y - thickness }, { size.x, thickness }, color);
			FillRect({ start.x, start.y }, { thickness, size.y }, color);
			FillRect({ start.x + size.x - thickness, start.y }, { thickness, size.y }, color);
		}

		void Renderer2D::FillRect(Math::vec2 start, Math::vec2 size, Math::vec3 color)
		{
			AddQuad({ { start.x, start.y, 0.0f },					color, { 0.0f, 0.0f }, 1.0f },
					{ { start.x + size.x, start.y, 0.0f },			color, { 1.0f, 0.0f }, 1.0f },
					{ { start.x + size.x, start.y + size.y, 0.0f }, color, { 1.0f, 1.0f }, 1.0f },
					{ { start.x, start.y + size.y, 0.0f },			color, { 0.0f, 1.0f }, 1.0f });
		}

		void Renderer2D::Render()
		{
			if (!m_IBO->GetSize())
				return;

			m_VAO->Bind();
			Renderer::DrawElements(GL_TRIANGLES, m_IBO->GetSize());
			m_VAO->Unbind();
		}

		void Renderer2D::PushIndices(int count)
		{
			unsigned int indices[] = {
				0, 1, 2,
				2, 3, 0
			};

			for (int i = 0; i < 6; i++) {
				m_quadind.push_back(count + indices[i]);
			}
		}

	}
}
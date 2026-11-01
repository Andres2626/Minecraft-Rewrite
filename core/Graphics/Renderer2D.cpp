#include "mc.h"

#include "Graphics/Renderer2D.h"

#include "Graphics/Font/IFont.h"
#include "Graphics/Shader/Shaderflags.h"
#include "Graphics/GL/GL.h"

#include "Utils/Util.h"

#define RENDERER2D_INITIAL_VERTICES 1000
#define RENDERER2D_INITIAL_INDICES 6000
#define RENDERER_MAX_TEXTURES 31

namespace MC 
{
	namespace Graphics
	{
		struct Renderer2D::PImpl
		{
			MeshData MeshData;
			std::vector<Texture*> Textures;
			std::unique_ptr<Mesh> Mesh;
		};

		Renderer2D::Renderer2D()
		{
			m_Impl = new PImpl();

			VertexLayout VL;
			VL.AddAttribute<Math::vec3>(SHADER_VERTEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
			VL.AddAttribute<Math::vec3>(SHADER_COLOR_BIT, sizeof(Vertex), (void*)offsetof(Vertex, color));
			VL.AddAttribute<Math::vec2>(SHADER_TEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
			VL.AddAttribute<float>(SHADER_BRIG_BIT, sizeof(Vertex), (void*)offsetof(Vertex, brig));
			VL.AddAttribute<float>(SHADER_TEXID_BIT, sizeof(Vertex), (void*)offsetof(Vertex, tid));
			m_Impl->Mesh = std::make_unique<Mesh>(VL);

			m_Impl->MeshData.vertices.reserve(RENDERER2D_INITIAL_VERTICES);
			m_Impl->MeshData.indices.reserve(RENDERER2D_INITIAL_INDICES);
		}

		Renderer2D::~Renderer2D()
		{
			if (m_Impl)
				delete m_Impl;
		}

		void Renderer2D::Begin()
		{
			m_Impl->MeshData.Clear();
		}

		void Renderer2D::End()
		{
			m_Impl->Mesh->Bind();
			m_Impl->Mesh->Build(&m_Impl->MeshData);
			m_Impl->Mesh->Unbind();
		}

		void Renderer2D::AddVertex(const Vertex& vert)
		{
			m_Impl->MeshData.vertices.push_back(vert);
		}

		void Renderer2D::AddQuad(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4)
		{
			int count = (int)m_Impl->MeshData.vertices.size();
			AddVertex(v1);
			AddVertex(v2);
			AddVertex(v3);
			AddVertex(v4);
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

			AddQuad({ { v0.x, v0.y, 0.0f }, color, { 0.0f, 0.0f }, 1.0f, 0.0f },
					{ { v1.x, v1.y, 0.0f }, color, { 1.0f, 0.0f }, 1.0f, 0.0f },
					{ { v2.x, v2.y, 0.0f }, color, { 1.0f, 1.0f }, 1.0f, 0.0f },
					{ { v3.x, v3.y, 0.0f }, color, { 0.0f, 1.0f }, 1.0f, 0.0f });
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
			AddQuad({ { start.x, start.y, 0.0f },					color, { 0.0f, 0.0f }, 1.0f, 0.0f},
					{ { start.x + size.x, start.y, 0.0f },			color, { 1.0f, 0.0f }, 1.0f, 0.0f },
					{ { start.x + size.x, start.y + size.y, 0.0f }, color, { 1.0f, 1.0f }, 1.0f, 0.0f },
					{ { start.x, start.y + size.y, 0.0f },			color, { 0.0f, 1.0f }, 1.0f, 0.0f });
		}

		void Renderer2D::DrawTextWS(IFont *font, Math::vec2 pos, Math::vec3 color, const char *str)
		{
			DrawText(font, pos + Math::vec2(1.0f, -9.0f), color, true, str);
			DrawText(font, pos + Math::vec2(0.0f, -8.0f), color, str);	
		}

		void Renderer2D::DrawText(IFont *font, Math::vec2 pos, Math::vec3 color, const char *str)
		{
			DrawText(font, pos, color, false, str);
		}

		void Renderer2D::DrawText(IFont *font, Math::vec2 pos, Math::vec3 color, bool dark, const char *str)
		{
			Texture *ftex = &font->GetTexture();
			float texid = SubmitTexture(ftex);
			mc_assert(texid > 0.0f, "invalid texture id");

			if (dark)
				color *= 0.25;

			int advance = 0;

            size_t len = strlen(str);
			for (int i = 0; i < len; i++)
			{
				const Glyph glyph = font->GetGlyph(str[i]);
				Vertex v1 = { { pos.x + advance, pos.y + 8, 0.0f }, color, { glyph.uvBegin.x, glyph.uvBegin.y }, 1.0f, texid };
				Vertex v2 = { { pos.x + advance + 8, pos.y + 8, 0.0f }, color, { glyph.uvEnd.x, glyph.uvBegin.y }, 1.0f, texid };
				Vertex v3 = { { pos.x + advance + 8, pos.y, 0.0f }, color, { glyph.uvEnd.x, glyph.uvEnd.y }, 1.0f, texid };
				Vertex v4 = { { pos.x + advance, pos.y, 0.0f }, color, { glyph.uvBegin.x, glyph.uvEnd.y }, 1.0f, texid };
				AddQuad(v1, v2, v3, v4);

				advance += glyph.advance;
			}
		}

		int Renderer2D::DrawFormatedText(IFont* font, Math::vec2 pos, Math::vec3 color, bool dark, const char* fmt, ...)
		{
			/* TODO: delete text limit */
			char *buff = (char*)malloc(255 * sizeof(char));
			if (!buff) {
				mc_fatal("error allocating text buffer");
				return 0;
			}

			va_list args;
			va_start(args, fmt);
			int ret = vsnprintf(buff, 255, fmt, args);
			va_end(args);
			DrawText(font, pos, color, dark, buff);
			free(buff);
			return ret;
		}

		int Renderer2D::DrawFormatedTextWS(IFont* font, Math::vec2 pos, Math::vec3 color, const char* fmt, ...)
		{
			/* TODO: delete text limit */
			char* buff = (char*)malloc(255 * sizeof(char));
			if (!buff) {
				mc_fatal("error allocating text buffer");
				return 0;
			}

			va_list args;
			va_start(args, fmt);
			int ret = vsnprintf(buff, 255, fmt, args);
			va_end(args);
			DrawTextWS(font, pos, color, buff);
			free(buff);
			return ret;
		}

		void Renderer2D::Render()
		{
			if (!m_Impl->Mesh->GetIndexCount())
				return;

			for (size_t i = 0; i < m_Impl->Textures.size(); i++)
				m_Impl->Textures[i]->Bind((u32t)i);

			Renderer::Disable(GL_DEPTH_TEST);
			m_Impl->Mesh->Render();
			Renderer::Enable(GL_DEPTH_TEST);

			for (size_t i = 0; i < m_Impl->Textures.size(); i++)
				m_Impl->Textures[i]->Unbind();

			m_Impl->Textures.clear();
		}

		void Renderer2D::PushIndices(int count)
		{
			unsigned int indices[] = {
				0, 1, 2,
				2, 3, 0
			};

			for (int i = 0; i < 6; i++) {
				m_Impl->MeshData.indices.push_back(count + indices[i]);
			}
		}

		float Renderer2D::SubmitTexture(Texture* tex)
        {
            for (size_t i = 0; i < m_Impl->Textures.size(); i++) {
                if (m_Impl->Textures[i] == tex)
                    return (float)(i + 1);
            }

            if (m_Impl->Textures.size() >= RENDERER_MAX_TEXTURES) {
                End();
                Render();
                Begin();
            }

            m_Impl->Textures.push_back(tex);

            return (float)m_Impl->Textures.size();
        }
	}
}
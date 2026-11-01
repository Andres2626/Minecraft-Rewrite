#pragma once
#include "common.h"

#include "Shader/Shader.h"
#include "Texture.h"
#include "Mesh.h"

#include "Utils/Util.h"

namespace MC 
{
	namespace Graphics 
	{
		struct Glyph;
		class IFont;

		class MC_API Renderer2D
		{
		private:
			struct PImpl;
			PImpl *m_Impl;
		public:
			Renderer2D();
			~Renderer2D();
		public:
			void Begin();
			void End();
		public:
			void AddVertex(const Vertex &vert);
			void AddQuad(const Vertex& v1, const Vertex &v2, const Vertex &v3, const Vertex &v4);
			void DrawLine(Math::vec2 start, Math::vec2 end, Math::vec3 color, float thickness = 0.3f);
			void DrawRect(Math::vec2 start, Math::vec2 size, Math::vec3 color, float thickness = 0.3f);
			void FillRect(Math::vec2 start, Math::vec2 size, Math::vec3 color);
			void DrawTextWS(IFont *font, Math::vec2 pos, Math::vec3 color, const char *str);
			void DrawText(IFont *font, Math::vec2 pos, Math::vec3 color, const char *str);
			void DrawText(IFont *font, Math::vec2 pos, Math::vec3 color, bool dark, const char *str);
			int DrawFormatedText(IFont* font, Math::vec2 pos, Math::vec3 color, bool dark, const char* fmt, ...);
			int DrawFormatedTextWS(IFont* font, Math::vec2 pos, Math::vec3 color, const char* fmt, ...);
		public:
			void Render();
		public:
			void PushIndices(int count);
		private:
			float SubmitTexture(Texture *tex);
		};
	}
}
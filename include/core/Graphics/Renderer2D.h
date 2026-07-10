#pragma once
#include "common.h"

#include "Mesh.h"

namespace MC 
{
	namespace Graphics 
	{
		class MC_API Renderer2D
		{
		private:
			/* quads */
			std::unique_ptr<VertexArray> m_VAO;
			std::unique_ptr<VertexBuffer> m_VBO;
			std::unique_ptr<IndexBuffer> m_IBO;
			std::vector<Vertex> m_quadvert;
			std::vector<unsigned int> m_quadind;
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
		public:
			void Render();
		public:
			void PushIndices(int count);
		};
	}
}
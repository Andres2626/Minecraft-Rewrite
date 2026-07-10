#pragma once
#include "common.h"

#include "Renderer.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexArray.h"

#include "Math/Math.h"

namespace MC 
{
	namespace Graphics 
	{
		struct Vertex
		{
			Math::vec3 pos;
			Math::vec3 color;
			Math::vec2 uv;
			float brig;
		};

		struct MeshData 
		{
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
		};

		class MC_API Mesh
		{
		private:
			MeshData *m_MeshData;
			std::unique_ptr<VertexArray> m_VAO;
			std::unique_ptr<VertexBuffer> m_VBO;
			std::unique_ptr<IndexBuffer> m_IBO;
		public:
			Mesh(MeshData *mesh);
			~Mesh();
		public:
			void Build();
			void Render() const;
		};
	}
}
#pragma once
#include "common.h"

#include "Renderer.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexArray.h"

#include "Math/Math.h"
#include "Utils/Util.h"

#include <memory>
#include <vector>

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
			float tid; /* texture index */
		};

		struct MC_API MeshData
		{
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;

			void Clear();
			void Release();
		};
		
		class MC_API Mesh
		{
		private:
			VertexLayout m_VL;
			std::unique_ptr<VertexArray> m_VAO;
			std::unique_ptr<VertexBuffer> m_VBO;
			std::unique_ptr<IndexBuffer> m_IBO;
		public:
			Mesh(VertexLayout &VL);
			~Mesh();
		public:
			void Build(MeshData *mesh);
			void Render() const;
		public:
			void Bind() const;
			void Unbind() const;
		public:
			u32t GetIndexCount() const;
		};
	}
}
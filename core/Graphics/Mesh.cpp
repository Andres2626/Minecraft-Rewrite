#include "Graphics/Mesh.h"

#include <gfx/glad.h>
#include "Graphics/Shader/Shaderflags.h"
#include "Graphics/Buffers/VertexBuffer.h"

namespace MC
{
	namespace Graphics
	{
		Mesh::Mesh(MeshData *meshData)
		{
			m_MeshData = meshData;
			m_VAO = std::make_unique<VertexArray>();
			m_IBO = std::make_unique<IndexBuffer>();
			m_VBO = std::make_unique<VertexBuffer>(GL_STATIC_DRAW);
		}

		Mesh::~Mesh()
		{

		}

		void Mesh::Build()
		{
			VertexLayout VL;
			m_VAO->Bind();
			m_VBO->Build(m_MeshData->vertices.size() * sizeof(Vertex), m_MeshData->vertices.data());
			m_IBO->Build(m_MeshData->indices.size(), m_MeshData->indices.data());
			VL.AddAttribute<Math::vec3>(SHADER_VERTEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
			VL.AddAttribute<Math::vec3>(SHADER_COLOR_BIT, sizeof(Vertex), (void*)offsetof(Vertex, color));
			VL.AddAttribute<Math::vec2>(SHADER_TEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
			VL.AddAttribute<float>(SHADER_BRIG_BIT, sizeof(Vertex), (void*)offsetof(Vertex, brig));
			m_VBO->SetVertexLayout(VL);
			m_VAO->Unbind();

			std::vector<Vertex>().swap(m_MeshData->vertices);
			std::vector<u32t>().swap(m_MeshData->indices);
		}

		void Mesh::Render() const
		{
			if (!m_IBO->GetSize())
				return;

			m_VAO->Bind();
			Renderer::DrawElements(GL_TRIANGLES, m_IBO->GetSize());
			m_VAO->Unbind();
		}

	}
}

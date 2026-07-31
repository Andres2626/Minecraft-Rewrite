
#include "Graphics/Mesh.h"

#include "Graphics/GL/GL.h"
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
			m_VBO->Build(m_MeshData->vertices.size() * sizeof(Vertex), m_MeshData->vertices.data());
			m_IBO->Build(m_MeshData->indices.size(), m_MeshData->indices.data());
			VL.AddAttribute<Math::vec3>(SHADER_VERTEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
			VL.AddAttribute<Math::vec3>(SHADER_COLOR_BIT, sizeof(Vertex), (void*)offsetof(Vertex, color));
			VL.AddAttribute<Math::vec2>(SHADER_TEX_BIT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
			VL.AddAttribute<float>(SHADER_BRIG_BIT, sizeof(Vertex), (void*)offsetof(Vertex, brig));
			m_VBO->SetVertexLayout(VL);

			std::vector<Vertex>().swap(m_MeshData->vertices);
			std::vector<u32t>().swap(m_MeshData->indices);
		}

		void Mesh::Render() const
		{
			if (!m_IBO->GetSize())
				return;

			Bind();
			Renderer::DrawElements(GL_TRIANGLES, m_IBO->GetSize());
			Unbind();
		}

		void Mesh::Bind() const
		{
			m_VAO->Bind();
		}

		void Mesh::Unbind() const
		{
			m_VAO->Unbind();
		}

	}
}

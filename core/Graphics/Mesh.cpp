#include "mc.h"

#include "Graphics/Mesh.h"

#include "Graphics/GL/GL.h"
#include "Graphics/Shader/Shaderflags.h"
#include "Graphics/Buffers/VertexBuffer.h"

namespace MC
{
	namespace Graphics
	{

		void MeshData::Clear()
		{
			vertices.clear();
			indices.clear();
		}

		void MeshData::Release()
		{
			std::vector<Vertex>().swap(vertices);
			std::vector<unsigned int>().swap(indices);
		}

		Mesh::Mesh(VertexLayout &VL)
		{
			m_VAO = std::make_unique<VertexArray>();
			m_IBO = std::make_unique<IndexBuffer>();
			m_VBO = std::make_unique<VertexBuffer>(GL_STATIC_DRAW);
			m_VL = VL;
		}

		Mesh::~Mesh()
		{
			
		}

		void Mesh::Build(MeshData *buff)
		{
			m_VBO->Bind();
			m_VBO->Build(buff->vertices.size() * sizeof(Vertex), buff->vertices.data());
			m_IBO->Bind();
			m_IBO->Build(buff->indices.size(), buff->indices.data());
			m_VBO->SetVertexLayout(m_VL);
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

		u32t Mesh::GetIndexCount() const
		{
			return m_IBO->GetSize();
		}
	}
}

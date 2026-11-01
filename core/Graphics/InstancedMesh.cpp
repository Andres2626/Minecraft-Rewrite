#include "mc.h"

#include "Graphics/InstancedMesh.h"

#include "Graphics/GL/GL.h"
#include "Graphics/Shader/Shaderflags.h"
#include "Graphics/Buffers/VertexBuffer.h"

namespace MC
{
	namespace Graphics
	{

		InstancedMesh::InstancedMesh(Mesh &mesh, VertexLayout &IVL, u32t instanceSize, u32t maxInstances)
			: m_Mesh(mesh)
		{
			m_IVL = IVL;
			m_InstanceSize = instanceSize;
			m_MaxInstances = maxInstances;
			m_InstanceCount = 0;
			m_InstanceBuffer = std::make_unique<VertexBuffer>(GL_DYNAMIC_DRAW);
		}

		InstancedMesh::~InstancedMesh()
		{
		}

		void InstancedMesh::Build()
		{
			m_InstanceBuffer->Resize(m_MaxInstances * m_InstanceSize);
			m_InstanceBuffer->SetVertexLayout(m_IVL);
		}

		void InstancedMesh::UpdateInstances(u32t size, const void *data)
		{
			if (!size || !data)
				return;

			m_InstanceCount = size;

			if (m_InstanceCount > m_MaxInstances) {
				m_MaxInstances *= 2;
				m_Mesh.Bind();
				Bind();
				Build();
				Unbind();
				m_Mesh.Unbind();
			}

			Bind();
			m_InstanceBuffer->Update(0, m_InstanceCount * m_InstanceSize, data);
			Unbind();
		}

		void InstancedMesh::Render() const
		{
			if (!m_InstanceCount)
				return;

			m_Mesh.Bind();
			Renderer::DrawElementsInstanced(GL_TRIANGLES, m_Mesh.GetIndexCount(), NULL, m_InstanceCount);
			m_Mesh.Unbind();
		}

		void InstancedMesh::Bind() const
		{
			m_InstanceBuffer->Bind();
		}

		void InstancedMesh::Unbind() const
		{
			m_InstanceBuffer->Unbind();
		}

		VertexBuffer &InstancedMesh::GetInstanceBuffer() const
		{
			return *m_InstanceBuffer;
		}
	}
}

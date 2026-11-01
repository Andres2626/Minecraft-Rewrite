#pragma once
#include "common.h"

#include "Mesh.h"
#include "Renderer.h"

#include "Math/Math.h"
#include "Utils/Util.h"

namespace MC 
{
	namespace Graphics 
	{
		class MC_API InstancedMesh
		{
		private:
			Mesh &m_Mesh;
			VertexLayout m_IVL;
			u32t m_InstanceSize;
			u32t m_MaxInstances;
			u32t m_InstanceCount;
			std::unique_ptr<VertexBuffer> m_InstanceBuffer;
		public:
			InstancedMesh(Mesh &mesh, VertexLayout &IVL, u32t instanceSize, u32t maxInstances);
			~InstancedMesh();
		public:
			void Build();
			void UpdateInstances(u32t size, const void *data);
			void Render() const;
		public:
			void Bind() const;
			void Unbind() const;
		public:
			VertexBuffer &GetInstanceBuffer() const;
		};
	}
}
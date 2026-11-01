#pragma once
#include "common.h"

#include "Mesh.h"

namespace MC 
{
	namespace Graphics 
	{
		class MC_API MeshFactory 
		{
		public:
			static void AddVertex(MeshData &data, const Vertex &v);
			static void AddQuad(MeshData &data, const Vertex &v0, const Vertex &v1, const Vertex &v2, const Vertex &v3);
			static void AddIndices(MeshData &data, int count);
		};
	}
}
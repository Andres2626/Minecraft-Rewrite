#include "mc.h"

#include "Graphics/MeshFactory.h"

#include <array>

namespace MC
{
	namespace Graphics
	{
		void MeshFactory::AddVertex(MeshData &data, const Vertex &v)
		{
			data.vertices.push_back(v);
		}

		void MeshFactory::AddQuad(MeshData &data, const Vertex &v0, const Vertex &v1, const Vertex &v2, const Vertex &v3)
		{
			AddVertex(data, v0);
			AddVertex(data, v1);
			AddVertex(data, v2);
			AddVertex(data, v3);
		}

		void MeshFactory::AddIndices(MeshData &data, int count)
		{
			unsigned int indices[] = {
				0, 1, 2,
				2, 3, 0
			};

			for (int i = 0; i < 6; i++) {
				data.indices.push_back(count + indices[i]);
			}
		}
	}
}

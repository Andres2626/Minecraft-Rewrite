#include "Graphics/MeshFactory.h"

#include <array>

namespace MC
{
	namespace Graphics
	{
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

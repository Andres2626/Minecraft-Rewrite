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
			static void AddIndices(MeshData& data, int count);
		};
	}
}
#pragma once
#include "Shader.h"

#include "common.h"

namespace MC 
{
	namespace Graphics 
	{
		class MC_API ShaderManager
		{
		public:
			static Shader &Get(const char *name);
			static void Register(const char *name, const char *virtpath);
		public:
			static void Bind(const char *name);
			static void Unbind();
		};
	}
}
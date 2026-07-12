#pragma once
#include "Shader.h"

#include "common.h"

namespace MC 
{
	namespace Graphics 
	{
		class MC_API ShaderManager
		{
		private:
			static ErrorHandler err;
			static std::unordered_map<mc_str, std::unique_ptr<Shader>> m_Shaders;
		public:
			static Shader &Get(const mc_str &name);
			static void Register(const mc_str &name, const mc_str &path);
		public:
			static void Bind(const mc_str &name);
			static void Unbind();
		};
	}
}
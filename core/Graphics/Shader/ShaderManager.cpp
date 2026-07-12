#include "Graphics/Shader/ShaderManager.h"

#define MC_LOG_PREFIX "ShaderManager"
#include "Log/Log.h"

#include <gfx/glad.h>

namespace MC 
{
	namespace Graphics 
	{
		std::unordered_map<mc_str, std::unique_ptr<Shader>> ShaderManager::m_Shaders;
		ErrorHandler ShaderManager::err;

		Shader &ShaderManager::Get(const mc_str &name)
		{
			auto it = m_Shaders.find(name);

			if (it == m_Shaders.end()) {
				mc_fatal("{} not registered.", name);
				err.SetError(ErrorType::ShaderRegisterError);
			}

			return *it->second;
		}

		void ShaderManager::Register(const mc_str &name, const mc_str &path)
		{
			auto it = m_Shaders.find(name);

			if (it != m_Shaders.end()) {
				mc_warn("{} already registered.", name);
				return;
			}

			std::unique_ptr<Shader> shader;
			shader = std::make_unique<Shader>(path);
			const Error &err = shader->GetError();
			if (err.num != ErrorType::NoError) {
				mc_error("error {}: {}", (u32t)err.num, err.str);
				return;
			}

			m_Shaders.insert({ name, std::move(shader) });
		}

		void ShaderManager::Bind(const mc_str &name)
		{
			Get(name).Bind();
		}

		void ShaderManager::Unbind()
		{
			glUseProgram(0);
		}
	}
}
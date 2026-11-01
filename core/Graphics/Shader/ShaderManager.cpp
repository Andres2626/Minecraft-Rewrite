#include "mc.h"

#include "Graphics/Shader/ShaderManager.h"

#include "Graphics/GL/GL.h"

#define MC_LOG_PREFIX "ShaderManager"
#include "Log/Log.h"

namespace MC 
{
	namespace Graphics 
	{
		static std::unordered_map<std::string, std::unique_ptr<Shader>> s_Shaders;

		Shader &ShaderManager::Get(const char *name)
		{
			auto it = s_Shaders.find(name);

#if 0
			if (it == s_Shaders.end()) {
				mc_fatal("{} not registered.", name);
				err.SetError(ErrorType::ShaderRegisterError);
			}
#endif 

			return *it->second;
		}

		void ShaderManager::Register(const char *name, const char *virtpath)
		{
			auto it = s_Shaders.find(name);

			if (it != s_Shaders.end()) {
				mc_warn("{} already registered.", name);
				return;
			}

			std::unique_ptr<Shader> shader;
			shader = std::make_unique<Shader>(virtpath);

#if 0
			const Error &err = shader->GetError();
			if (err.num != ErrorType::NoError) {
				mc_error("error {}: {}", (u32t)err.num, err.str);
				return;
			}
#endif

			s_Shaders.insert({ name, std::move(shader) });
		}

		void ShaderManager::Bind(const char *name)
		{
			Get(name).Bind();
		}

		void ShaderManager::Unbind()
		{
			glUseProgram(0);
		}
	}
}
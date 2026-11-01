#pragma once
#include "Shaderflags.h"

#include "common.h"
#include "Math/Math.h"
#include "Utils/Util.h"
#include "Utils/Error.h"
#include "Utils/NonCopyable.h"

#include "Graphics/GL/GL.h"

namespace MC 
{
	namespace Graphics 
	{
		enum class ShaderType
		{
			UNKNOWN = -1,
			VERTEX, 
			FRAGMENT
		};

		enum class ShaderUniformType
		{
			BOOL,
			INT,
			FLOAT,
			VEC2,
			VEC3,
			VEC4,
			MAT2,
			MAT3,
			MAT4,
		};

		class MC_API Shader : public NonCopyable
		{
		private:
			struct PImpl;
			PImpl *m_Impl;
		public:
			/* load shader from file */
			Shader(const char *virtpath);
			~Shader();
		public:
			/* enable program */
			void Bind() const;

			/* disable program */
			void Unbind() const;
		public:
			template <typename T>
			void SetUniform(const char *name, const T &value)
			{
				SetUniformInternal(name, (void*)&value, 1, GetUniformType<T>());
			}

			template <typename T>
			void SetUniformArray(const char *name, u32t count, const T *value)
			{
				SetUniformInternal(name, (void*)value, count, GetUniformType<T>());
			}
		private:
			u32t Load(u32t type, const char *source);
			u32t Create(const char *vertex_source, const char *fragmement_source);
		private:
			template <typename T>
			static ShaderUniformType GetUniformType()
			{
				mc_assert(false, "not implemented uniform type");
				return ShaderUniformType::INT;
			}
		private:
			void SetUniformInternal(const char *name, void *data, u32t count, ShaderUniformType type);
			int GetUniformLocation(const char *name);
		};

		template<>
		ShaderUniformType MC_API Shader::GetUniformType<bool>();

		template<>
		ShaderUniformType MC_API Shader::GetUniformType<int>();

		template<>
		ShaderUniformType MC_API Shader::GetUniformType<float>();

		template<>
		ShaderUniformType MC_API Shader::GetUniformType<Math::vec2>();

		template<>
		ShaderUniformType MC_API Shader::GetUniformType<Math::vec3>();

		template<>
		ShaderUniformType MC_API Shader::GetUniformType<Math::vec4>();

		template<>
		ShaderUniformType MC_API Shader::GetUniformType<Math::mat2>();

		template<>
		ShaderUniformType MC_API Shader::GetUniformType<Math::mat3>();

		template<>
		ShaderUniformType MC_API Shader::GetUniformType<Math::mat4>();
	}
}
#pragma once
#include "Shaderflags.h"

#include "common.h"
#include "Math/Math.h"
#include "Log/Log.h"

#include <glad/glad.h>

namespace MC 
{
	namespace Graphics 
	{

		struct MC_API ShaderSources 
		{
			rd_str_t Vertex;
			rd_str_t Fragment;
		};

		class MC_API Shader 
		{
		private:
			rd_uint8_t m_ShaderID;
			rd_str_t m_Path;
		public:
			/* load shader from file */
			Shader(const rd_str_t& filepath);
			~Shader();
		public:
			/* enable program */
			void Bind() const;

			/* disable program */
			void Unbind() const;
		public:
			void SetBool(const rd_str_t& name, bool value);
			void SetInt(const rd_str_t& name, int value);
			void SetFloat(const rd_str_t& name, float value);
			void SetVec2(const rd_str_t& name, const Math::vec2& value);
			void SetVec3(const rd_str_t& name, const Math::vec3& value);
			void SetVec4(const rd_str_t& name, const Math::vec4& value);
			void Set2x2(const rd_str_t& name, const Math::mat2& value);
			void Set3x3(const rd_str_t& name, const Math::mat3& value);
			void Set4x4(const rd_str_t& name, const Math::mat4& value);
		private:
			ShaderSources ParseFromFile(const rd_str_t& path);
			rd_uint8_t Load(rd_uint8_t type, const rd_str_t& source);
			rd_uint8_t Create(const rd_str_t& vertex_source, const rd_str_t& fragmement_source);
		private:
			rd_int_t GetUniformLocation(rd_str_t name);
		};
	}
}
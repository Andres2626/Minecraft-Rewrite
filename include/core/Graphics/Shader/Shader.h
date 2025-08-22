#pragma once
#include "Shaderflags.h"

#include "common.h"
#include "Math/Math.h"
#include "Log/Log.h"

#include <gfx/glad.h>

namespace MC 
{
	namespace Graphics 
	{

		struct MC_API ShaderSources 
		{
			mc_str Vertex;
			mc_str Fragment;
		};

		class MC_API Shader 
		{
		private:
			mc_u8 m_ShaderID;
			mc_str m_Path;
		public:
			/* load shader from file */
			Shader(const mc_str& filepath);
			~Shader();
		public:
			/* enable program */
			void Bind() const;

			/* disable program */
			void Unbind() const;
		public:
			void SetBool(const mc_str& name, bool value);
			void SetInt(const mc_str& name, int value);
			void SetFloat(const mc_str& name, float value);
			void SetVec2(const mc_str& name, const Math::vec2& value);
			void SetVec3(const mc_str& name, const Math::vec3& value);
			void SetVec4(const mc_str& name, const Math::vec4& value);
			void Set2x2(const mc_str& name, const Math::mat2& value);
			void Set3x3(const mc_str& name, const Math::mat3& value);
			void Set4x4(const mc_str& name, const Math::mat4& value);
		private:
			ShaderSources ParseFromFile(const mc_str& path);
			mc_u8 Load(mc_u8 type, const mc_str& source);
			mc_u8 Create(const mc_str& vertex_source, const mc_str& fragmement_source);
		private:
			mc_int GetUniformLocation(mc_str name);
		};
	}
}
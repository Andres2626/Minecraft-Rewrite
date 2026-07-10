#pragma once
#include "Shaderflags.h"

#include "common.h"
#include "Math/Math.h"
#include "Utils/Error.h"

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
			u32t m_ShaderID;
			mc_str m_Path;
			ErrorHandler err;
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
			void SetBool(const mc_str &name, bool value);
			void SetInt(const mc_str &name, int value);
			void SetFloat(const mc_str &name, float value);
			void SetVec2(const mc_str &name, const Math::vec2 &value);
			void SetVec3(const mc_str &name, const Math::vec3 &value);
			void SetVec4(const mc_str &name, const Math::vec4 &value);
			void Set2x2(const mc_str &name, const Math::mat2 &value);
			void Set3x3(const mc_str &name, const Math::mat3 &value);
			void Set4x4(const mc_str &name, const Math::mat4 &value);
		private:
			ShaderSources ParseFromFile(const mc_str &path);
			u32t Load(u32t type, const mc_str &source);
			u32t Create(const mc_str &vertex_source, const mc_str &fragmement_source);
		private:
			i32t GetUniformLocation(mc_str name);
		public:
			Error &GetError() { return err.GetError(); }
		};
	}
}
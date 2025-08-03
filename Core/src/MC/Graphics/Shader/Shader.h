#pragma once
#include "Shaderflags.h"

#include "common.h"
#include "Math/Math.h"
#include "Log/Log.h"

#include <glad/glad.h>

namespace MC {
	namespace Graphics {

		struct RD_API ShaderSources {
			std::string Vertex;
			std::string Fragment;
		};

		class RD_API Shader {
		private:
			GLuint m_ShaderID;
			std::string m_Path;
		public:
			/* load shader from file */
			Shader(std::string filepath);
			~Shader();
		public:
			/* enable program */
			void Bind() const;

			/* disable program */
			void Unbind() const;
		public:
			void SetBool(const std::string& name, bool value);
			void SetInt(const std::string& name, int value);
			void SetFloat(const std::string& name, float value);
			void SetVec2(const std::string& name, const Math::vec2& value);
			void SetVec3(const std::string& name, const Math::vec3& value);
			void SetVec4(const std::string& name, const Math::vec4& value);
			void Set2x2(const std::string& name, const Math::mat2& value);
			void Set3x3(const std::string& name, const Math::mat3& value);
			void Set4x4(const std::string& name, const Math::mat4& value);
		private:
			ShaderSources ParseFromFile(const std::string& path);
			GLuint Load(GLuint type, const std::string& source);
			GLuint Create(const std::string& vertex_source, const std::string& fragmement_source);
		private:
			GLint GetUniformLocation(std::string name);
		};
	}
}
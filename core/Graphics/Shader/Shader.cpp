
#include "Graphics/Shader/Shader.h"

#define MC_LOG_PREFIX "Shader"
#include "Log/Log.h"

#include <gfx/glad.h>

namespace MC 
{
	namespace Graphics 
	{
		Shader::Shader(const mc_str &filepath)
			: m_Path(filepath) 
		{
			ShaderSources shaders = ParseFromFile(filepath);
			m_ShaderID = Create(shaders.Vertex, shaders.Fragment);
			mc_info("path={}", m_Path);
		}

		Shader::~Shader() 
		{
			glDeleteShader(m_ShaderID);
		}

		void Shader::Bind() const 
		{
			glUseProgram(m_ShaderID);
		}

		void Shader::Unbind() const 
		{
			glUseProgram(0);
		}

		void Shader::SetBool(const mc_str &name, bool value)
		{
			i32t location = GetUniformLocation(name);
			if (location != -1)
				glUniform1i(location, value);
		}

		void Shader::SetInt(const mc_str &name, int value)
		{
			i32t location = GetUniformLocation(name);
			if (location != -1)
				glUniform1i(location, value);
		}

		void Shader::SetFloat(const mc_str &name, float value)
		{
			i32t location = GetUniformLocation(name);
			if (location != -1) 
				glUniform1f(location, value);
		}

		void Shader::SetVec2(const mc_str &name, const Math::vec2 &value)
		{
			i32t location = GetUniformLocation(name);
			if (location != -1)
				glUniform2f(location, value.x, value.y);
		}

		void Shader::SetVec3(const mc_str &name, const Math::vec3 &value)
		{
			i32t location = GetUniformLocation(name);
			if (location != -1)
				glUniform3f(location, value.x, value.y, value.z);
		}

		void Shader::SetVec4(const mc_str &name, const Math::vec4 &value)
		{
			i32t location = GetUniformLocation(name);
			if (location != -1) 
				glUniform4f(location, value.x, value.y, value.z, value.w);
		}

		void Shader::Set2x2(const mc_str &name, const Math::mat2 &value)
		{
			i32t location = GetUniformLocation(name);
			if (location != -1)
				glUniformMatrix2fv(location, 1, GL_FALSE, (const float*)&value);
		}

		void Shader::Set3x3(const mc_str &name, const Math::mat3 &value)
		{
			i32t location = GetUniformLocation(name);
			if (location != -1)
				glUniformMatrix3fv(location, 1, GL_FALSE, (const float*)&value);
		}

		void Shader::Set4x4(const mc_str &name, const Math::mat4 &value)
		{
			i32t location = GetUniformLocation(name);
			if (location != -1)
				glUniformMatrix4fv(location, 1, GL_FALSE, (const float*)&value);
		}

		ShaderSources Shader::ParseFromFile(const mc_str &path)
		{
			mc_str line;
			std::stringstream ss[2];
			std::ifstream stream(path);

			if (!stream) {
				err.SetError(ErrorType::FileNotFound);
				mc_fatal("failed to open shader file \"{}\"\n", path);
			}

			enum class ShaderType 
			{
				UNKNOUN = -1, VERTEX = 0, FRAGMENT = 1
			};

			ShaderType type = ShaderType::UNKNOUN;

			while (getline(stream, line)) {
				if (line.find("#shader") != mc_str::npos) {
					if (line.find("vertex") != mc_str::npos)
						type = ShaderType::VERTEX;
					else if (line.find("fragment") != mc_str::npos)
						type = ShaderType::FRAGMENT;
				}
				else
					ss[(int)type] << line << "\n";
			}

			return{
				ss[(int)ShaderType::VERTEX].str(),
				ss[(int)ShaderType::FRAGMENT].str()
			};
		}

		u32t Shader::Load(u32t type, const mc_str &source)
		{
			int result = GL_FALSE;
			unsigned int id = glCreateShader(type);
			const char *sourceStr = source.c_str();
			int len;

			glShaderSource(id, 1, &sourceStr, nullptr);
			glCompileShader(id);
			glGetShaderiv(id, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE) {
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
				std::vector<char> error(len);
				glGetShaderInfoLog(id, len, &len, &error[0]);

				if (type)
					err.SetError(ErrorType::ShaderCompileError);

				switch (type) {
				case GL_VERTEX_SHADER:
					mc_error("vertex shader error");
					break;
				case GL_FRAGMENT_SHADER:
					mc_error("fragment shader error");
					break;
				default:
					mc_error("unknoun shader {}", type);
					break;
				}

				mc_error("{}", &error[0]);

				glDeleteShader(id);
				return 0;
			}

			return id;
		}

		u32t Shader::Create(const mc_str &vertex_source, const mc_str &fragmement_source)
		{
			u32t program = glCreateProgram();
			u32t vs = Load(GL_VERTEX_SHADER, vertex_source);
			u32t fs = Load(GL_FRAGMENT_SHADER, fragmement_source);

			glAttachShader(program, vs);
			glAttachShader(program, fs);
			glLinkProgram(program);
			glValidateProgram(program);
			glDeleteShader(vs);
			glDeleteShader(fs);

			return program;
		}

		i32t Shader::GetUniformLocation(mc_str name)
		{
			int location = glGetUniformLocation(m_ShaderID, name.c_str());
#if defined (MC_USE_DEBUG)
			if (location == -1)
				mc_debug("shader location of \"{}\" not found", name.c_str());
#endif
			return location;
		}

	}
}
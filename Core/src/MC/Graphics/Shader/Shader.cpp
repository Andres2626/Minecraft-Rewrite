#include "Shader.h"

#include <glad/glad.h>

namespace MC 
{
	namespace Graphics 
	{

		Shader::Shader(const rd_str_t& filepath)
			: m_Path(filepath) 
		{
			ShaderSources shaders = ParseFromFile(filepath);
			m_ShaderID = Create(shaders.Vertex, shaders.Fragment);
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

		void Shader::SetBool(const rd_str_t& name, bool value) 
		{
			int location = GetUniformLocation(name);
			if (location != -1) {
				glUniform1i(location, value);
			}
		}

		void Shader::SetInt(const rd_str_t& name, int value) 
		{
			int location = GetUniformLocation(name);
			if (location != -1) {
				glUniform1i(location, value);
			}
		}

		void Shader::SetFloat(const rd_str_t& name, float value) 
		{
			int location = GetUniformLocation(name);
			if (location != -1) {
				glUniform1f(location, value);
			}
		}

		void Shader::SetVec2(const rd_str_t& name, const Math::vec2& value) 
		{
			int location = GetUniformLocation(name);
			if (location != -1) {
				glUniform2f(location, value.x, value.y);
			}
		}

		void Shader::SetVec3(const rd_str_t& name, const Math::vec3& value) 
		{
			int location = GetUniformLocation(name);
			if (location != -1) {
				glUniform3f(location, value.x, value.y, value.z);
			}
		}

		void Shader::SetVec4(const rd_str_t& name, const Math::vec4& value) 
		{
			int location = GetUniformLocation(name);
			if (location != -1) {
				glUniform4f(location, value.x, value.y, value.z, value.w);
			}
		}

		void Shader::Set2x2(const rd_str_t& name, const Math::mat2& value) 
		{
			int location = GetUniformLocation(name);
			glUniformMatrix2fv(location, 1, GL_FALSE, (const float*)&value);
		}

		void Shader::Set3x3(const rd_str_t& name, const Math::mat3& value) 
		{
			int location = GetUniformLocation(name);
			glUniformMatrix3fv(location, 1, GL_FALSE, (const float*)&value);
		}

		void Shader::Set4x4(const rd_str_t& name, const Math::mat4& value) 
		{
			int location = GetUniformLocation(name);
			glUniformMatrix4fv(location, 1, GL_FALSE, (const float*)&value);
		}

		ShaderSources Shader::ParseFromFile(const rd_str_t& path) 
		{
			rd_str_t line;
			std::stringstream ss[2];
			std::ifstream stream(path);

			if (!stream) 
				MC_FATAL << "Failed to open shader file \"" << path << "\"";

			enum class ShaderType 
			{
				UNKNOUN = -1, VERTEX = 0, FRAGMENT = 1
			};

			ShaderType type = ShaderType::UNKNOUN;

			while (getline(stream, line)) {
				if (line.find("#shader") != rd_str_t::npos) {
					if (line.find("vertex") != rd_str_t::npos)
						type = ShaderType::VERTEX;
					else if (line.find("fragment") != rd_str_t::npos)
						type = ShaderType::FRAGMENT;
				}
				else {
					ss[(int)type] << line << "\n";
				}
			}

			return{
				ss[(int)ShaderType::VERTEX].str(),
				ss[(int)ShaderType::FRAGMENT].str()
			};
		}

		rd_uint8_t Shader::Load(rd_uint8_t type, const rd_str_t& source)
		{
			int result = GL_FALSE;
			unsigned int id = glCreateShader(type);
			const char* sourceStr = source.c_str();
			int len;

			glShaderSource(id, 1, &sourceStr, nullptr);
			glCompileShader(id);
			glGetShaderiv(id, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE) {
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
				std::vector<char> error(len);
				glGetShaderInfoLog(id, len, &len, &error[0]);

				switch (type) {
				case GL_VERTEX_SHADER:
					MC_ERROR << "Vertex shader error! deleting...\n";
					break;
				case GL_FRAGMENT_SHADER:
					MC_ERROR << "Fragment shader error! deleting...\n";
					break;
				default:
					MC_ERROR << "Unknoun shader error! deleting...\n";
					break;
				}

				printf("%s", &error[0]);

				glDeleteShader(id);
				return 0;
			}

			return id;
		}

		GLuint Shader::Create(const rd_str_t& vertex_source, const rd_str_t& fragmement_source) 
		{
			rd_uint8_t program = glCreateProgram();
			rd_uint8_t vs = Load(GL_VERTEX_SHADER, vertex_source);
			rd_uint8_t fs = Load(GL_FRAGMENT_SHADER, fragmement_source);

			glAttachShader(program, vs);
			glAttachShader(program, fs);
			glLinkProgram(program);
			glValidateProgram(program);
			glDeleteShader(vs);
			glDeleteShader(fs);

			return program;
		}

		GLint Shader::GetUniformLocation(rd_str_t name) 
		{
			int location = glGetUniformLocation(m_ShaderID, name.c_str());
#ifdef _DEBUG
			if (location == -1) {
				MC_DEBUG << "Shader Location of: " << name.c_str() << " not found!, Skipping...\n";
			}
#endif
			return location;
		}

	}
}
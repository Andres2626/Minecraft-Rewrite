#include "Shader.h"

namespace MC {
	namespace Graphics {

		Shader::Shader(std::string filepath)
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

		void Shader::SetBool(const std::string& name, bool value) 
		{
			int location = GetUniformLocation(name);
			if (location != -1) {
				glUniform1i(location, value);
			}
		}

		void Shader::SetInt(const std::string& name, int value) 
		{
			int location = GetUniformLocation(name);
			if (location != -1) {
				glUniform1i(location, value);
			}
		}

		void Shader::SetFloat(const std::string& name, float value) 
		{
			int location = GetUniformLocation(name);
			if (location != -1) {
				glUniform1f(location, value);
			}
		}

		void Shader::SetVec2(const std::string& name, const glm::vec2& value) 
		{
			int location = GetUniformLocation(name);
			if (location != -1) {
				glUniform2f(location, value.x, value.y);
			}
		}

		void Shader::SetVec3(const std::string& name, const glm::vec3& value) 
		{
			int location = GetUniformLocation(name);
			if (location != -1) {
				glUniform3f(location, value.x, value.y, value.z);
			}
		}

		void Shader::SetVec4(const std::string& name, const glm::vec4& value) 
		{
			int location = GetUniformLocation(name);
			if (location != -1) {
				glUniform4f(location, value.x, value.y, value.z, value.w);
			}
		}

		void Shader::Set2x2(const std::string& name, const glm::mat2& value) 
		{
			int location = GetUniformLocation(name);
			glUniformMatrix2fv(location, 1, GL_FALSE, (const float*)&value);
		}

		void Shader::Set3x3(const std::string& name, const glm::mat3& value) 
		{
			int location = GetUniformLocation(name);
			glUniformMatrix3fv(location, 1, GL_FALSE, (const float*)&value);
		}

		void Shader::Set4x4(const std::string& name, const glm::mat4& value) 
		{
			int location = GetUniformLocation(name);
			glUniformMatrix4fv(location, 1, GL_FALSE, (const float*)&value);
		}

		ShaderSources Shader::ParseFromFile(const std::string& path) 
		{
			std::string line;
			std::stringstream ss[2];
			std::ifstream stream(path);

			MC_ERRORCHK(stream, "Error opening Shader file. %s", path)

				enum class ShaderType {
				UNKNOUN = -1, VERTEX = 0, FRAGMENT = 1
			};

			ShaderType type = ShaderType::UNKNOUN;

			while (getline(stream, line)) {
				if (line.find("#shader") != std::string::npos) {
					if (line.find("vertex") != std::string::npos) {
						type = ShaderType::VERTEX;
					}
					else if (line.find("fragment") != std::string::npos) {
						type = ShaderType::FRAGMENT;
					}
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

		GLuint Shader::Load(GLuint type, const std::string& source) 
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

				if (type == GL_VERTEX_SHADER) {
					MC_ERROR("Error to compile Vertex shader! deleting...\n");
				}
				else if (type == GL_FRAGMENT_SHADER) {
					MC_ERROR("Error to compile Fragment shader! deleting...\n");
				}
				else {
					MC_ERROR("Error to compile Unknoun shader! deleting...\n");
				}

				printf("%s", &error[0]);

				glDeleteShader(id);
				return 0;
			}

			return id;
		}

		GLuint Shader::Create(const std::string& vertex_source, const std::string& fragmement_source) 
		{
			unsigned int program = glCreateProgram();
			unsigned int vs = Load(GL_VERTEX_SHADER, vertex_source);
			unsigned int fs = Load(GL_FRAGMENT_SHADER, fragmement_source);

			glAttachShader(program, vs);
			glAttachShader(program, fs);
			glLinkProgram(program);
			glValidateProgram(program);
			glDeleteShader(vs);
			glDeleteShader(fs);

			return program;
		}

		GLint Shader::GetUniformLocation(std::string name) 
		{
			int location = glGetUniformLocation(m_ShaderID, name.c_str());
#ifdef _DEBUG
			if (location == -1) {
				MC_DEBUG("Shader Location of: %s not found!, Skipping...\n", name.c_str());
			}
#endif
			return location;
		}

	}
}
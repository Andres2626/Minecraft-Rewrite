#include "mc.h"

#include "Graphics/Shader/Shader.h"

#include "Graphics/GL/GL.h"

#define MC_LOG_PREFIX "Shader"
#include "Log/Log.h"

#include "Filesystem/VirtualFileSystem.h"
#include "Utils/Util.h"

namespace MC 
{
	namespace Graphics 
	{
		struct Shader::PImpl
		{
			u32t ID;
			const char* Path;
			std::unordered_map<std::string, int> UniformLocations;
		};

		template<>
		ShaderUniformType Shader::GetUniformType<bool>()
		{
			return ShaderUniformType::BOOL;
		}

		template<>
		ShaderUniformType Shader::GetUniformType<int>()
		{
			return ShaderUniformType::INT;
		}

		template<>
		ShaderUniformType Shader::GetUniformType<float>()
		{
			return ShaderUniformType::FLOAT;
		}

		template<>
		ShaderUniformType Shader::GetUniformType<Math::vec2>()
		{
			return ShaderUniformType::VEC2;
		}

		template<>
		ShaderUniformType Shader::GetUniformType<Math::vec3>()
		{
			return ShaderUniformType::VEC3;
		}

		template<>
		ShaderUniformType Shader::GetUniformType<Math::vec4>()
		{
			return ShaderUniformType::VEC4;
		}

		template<>
		ShaderUniformType Shader::GetUniformType<Math::mat2>()
		{
			return ShaderUniformType::MAT2;
		}

		template<>
		ShaderUniformType Shader::GetUniformType<Math::mat3>()
		{
			return ShaderUniformType::MAT3;
		}

		template<>
		ShaderUniformType Shader::GetUniformType<Math::mat4>()
		{
			return ShaderUniformType::MAT4;
		}

		struct MC_API ShaderSources
		{
			std::string Vertex;
			std::string Fragment;
		};

		ShaderSources ParseFromFile(const char* virtpath)
		{
			std::string out;
			std::string line;
			std::stringstream ss[2];

			File::FileHandle fp = File::VirtualFileSystem::Open(virtpath, File::FileMode::Read);
			if (fp.handle == MC_FILE_INVALID) {
				//err.SetError(ErrorType::FileNotFound);
				mc_fatal("failed to open shader file \"{}\"\n", virtpath);
				return {};
			}

			size_t sz = File::VirtualFileSystem::Size(fp);
			out.resize(sz);

			if (!File::VirtualFileSystem::Read(fp, out.data(), sz)) {
				File::VirtualFileSystem::Close(fp);
				//err.SetError(ErrorType::AssetLoad);
				mc_fatal("failed to read shader file \"{}\"\n", virtpath);
				return {};
			}

			File::VirtualFileSystem::Close(fp);

			std::stringstream stream(out);
			ShaderType type = ShaderType::UNKNOWN;

			size_t offset = 0;
			while (getline(stream, line)) {
				/* detele spaces at the begin line */
				size_t start = line.find_first_not_of(" \t\r\n");
				if (start == std::string::npos)
					continue;

				/* ignore comments */
				std::string st = line.substr(start);
				if (st.rfind("//", 0) == 0)
					continue;

				/* parse #shader directive */
				if (st.find("#shader") != std::string::npos) {
					if (st.find("vertex") != std::string::npos)
						type = ShaderType::VERTEX;
					else if (st.find("fragment") != std::string::npos)
						type = ShaderType::FRAGMENT;
					continue;
				}

				if (type != ShaderType::UNKNOWN)
					ss[(int)type] << line << "\n";
			}

			return{
				ss[(int)ShaderType::VERTEX].str(),
				ss[(int)ShaderType::FRAGMENT].str()
			};
		}

		Shader::Shader(const char *virtpath)
		{
			m_Impl = new PImpl();
			m_Impl->Path = virtpath;

			ShaderSources shaders = ParseFromFile(virtpath);
			m_Impl->ID = Create(shaders.Vertex.c_str(), shaders.Fragment.c_str());

			mc_info("path={}", m_Impl->Path);
		}

		Shader::~Shader() 
		{
			if (m_Impl->ID != 0)
				glDeleteProgram(m_Impl->ID);

			if (m_Impl)
				delete m_Impl;
		}

		void Shader::Bind() const 
		{
			glUseProgram(m_Impl->ID);
		}

		void Shader::Unbind() const 
		{
			glUseProgram(0);
		}

		u32t Shader::Load(u32t type, const char *source)
		{
			int result = GL_FALSE;
			unsigned int id = glCreateShader(type);
			int len = 0;

			glShaderSource(id, 1, &source, nullptr);
			glCompileShader(id);
			glGetShaderiv(id, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE) {
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
				std::vector<char> error(len);
				glGetShaderInfoLog(id, len, &len, &error[0]);

				//if (type)
					//err.SetError(ErrorType::ShaderCompileError);

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

		u32t Shader::Create(const char *vertex_source, const char *fragmement_source)
		{
			u32t program = glCreateProgram();
			u32t vs = Load(GL_VERTEX_SHADER, vertex_source);
			u32t fs = Load(GL_FRAGMENT_SHADER, fragmement_source);

			if (!vs || !fs)
				return 0;

			glAttachShader(program, vs);
			glAttachShader(program, fs);
			glLinkProgram(program);
			glValidateProgram(program);
			glDeleteShader(vs);
			glDeleteShader(fs);

			return program;
		}

		void Shader::SetUniformInternal(const char *name, void *data, u32t count, ShaderUniformType type)
		{
			int location = GetUniformLocation(name);
			if (location == -1 || !data || !count)
				return; 

			switch (type) {
			case ShaderUniformType::BOOL: {
				glUniform1i(location, *static_cast<const bool*>(data) ? 1 : 0);
				break;
			}
			case ShaderUniformType::INT: {
				glUniform1i(location, *static_cast<const int*>(data));
				break;
			}
			case ShaderUniformType::FLOAT: {
				glUniform1f(location, *static_cast<const float*>(data));
				break;
			}
			case ShaderUniformType::VEC2: {
				const float *vec = static_cast<const float*>(data);
				glUniform2fv(location, count, vec);
				break;
			}
			case ShaderUniformType::VEC3: {
				const float *vec = static_cast<const float*>(data);
				glUniform3fv(location, count, vec);
				break;
			}
			case ShaderUniformType::VEC4: {
				const float *vec = static_cast<const float*>(data);
				glUniform4fv(location, count, vec);
				break;
			}
			case ShaderUniformType::MAT2: {
				const float *mat = static_cast<const float*>(data);
				glUniformMatrix2fv(location, count, GL_FALSE, mat);
				break;
			}
			case ShaderUniformType::MAT3: {
				const float *mat = static_cast<const float*>(data);
				glUniformMatrix3fv(location, count, GL_FALSE, mat);
				break;
			}
			case ShaderUniformType::MAT4: {
				const float *mat = static_cast<const float*>(data);
				glUniformMatrix4fv(location, count, GL_FALSE, mat);
				break;
			}
			default:
				mc_assert(false, "not implemented uniform type");
			}
		}
		
		int Shader::GetUniformLocation(const char *name)
		{
			if (m_Impl->UniformLocations.find(name) != m_Impl->UniformLocations.end())
				return m_Impl->UniformLocations[name];

			int location = glGetUniformLocation(m_Impl->ID, name);
#if defined (MC_USE_DEBUG)
			if (location == -1)
				mc_debug("shader uniform name \"{}\" does not exist", name);
#endif
			m_Impl->UniformLocations[name] = location;
			return location;
		}

	}
}
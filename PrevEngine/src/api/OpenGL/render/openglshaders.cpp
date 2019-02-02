#include "pch.h"
#include "openglshaders.h"

#include <glad/glad.h>

namespace prev {

	Shader* Shader::Create(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile) {
		Shader* shader = new opengl::OpenGLShaders(vertexShaderFile, fragmentShaderFile, geometryShaderFile);
		shader->LoadShader();
		return shader;
	}

	namespace opengl {

		OpenGLShaders::OpenGLShaders(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile) :
			Shader(vertexShaderFile, fragmentShaderFile, geometryShaderFile) {
		}

		OpenGLShaders::~OpenGLShaders() {
			glDeleteProgram(m_ProgramID);
		}

		void OpenGLShaders::UseShader() const {
			glUseProgram(m_ProgramID);
		}

		bool OpenGLShaders::ReadShader(std::string vShaderPath, std::string fShaderPath, std::string gShaderPath) {
			bool status = true;
			int vShaderID = -1, fShaderID = -1, gShaderID = -1;
			if ((vShaderID = ReadShaderFile(vShaderPath, VERTEX_SHADER)) == -1) {
				status = false;
			}
			if ((fShaderID = ReadShaderFile(fShaderPath, FRAGMENT_SHADER)) == -1) {
				status = false;
			}
			if (gShaderPath.size() != 0 && (gShaderID = ReadShaderFile(gShaderPath, GEOMETRY_SHADER)) == -1) {
				status = false;
			}
			m_AttachedShaders = 0;
			m_AttachedShaders |= VERTEX_SHADER;
			m_AttachedShaders |= FRAGMENT_SHADER;
			if (gShaderPath.size() != 0) {
				m_AttachedShaders |= GEOMETRY_SHADER;
			}
			if (status) {
				status = CreateProgram({ vShaderID, fShaderID, gShaderID });
			}
			glDeleteShader(vShaderID);
			glDeleteShader(fShaderID);
			glDeleteShader(gShaderID);
			return status;
		}

		bool OpenGLShaders::CreateProgram(std::initializer_list<int> shaders) {
			m_ProgramID = glCreateProgram();
			for (auto shader : shaders) {
				if (shader != -1) {
					glAttachShader(m_ProgramID, shader);
				}
			}
			glLinkProgram(m_ProgramID);
			int success;
			char infoLog[1024];
			glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(m_ProgramID, 1024, NULL, infoLog);
				PV_CORE_ERROR("Program Link Failed");
				PV_CORE_ERROR("%s", infoLog);
				PV_CORE_ERROR("-- --------------------------------------------------- --");
				glDeleteProgram(m_ProgramID);
				return false;
			}
			return true;
		}

		int OpenGLShaders::ReadShaderFile(std::string shaderPath, ShaderTypes type) {
			std::ifstream shaderFile(shaderPath);
			if (!shaderFile) {
				PV_CORE_ERROR("Unable to open shader file : %s", shaderPath.c_str());
				return -1;
			}
			std::string shaderCode;
			{
				std::stringstream shaderstream;
				shaderstream << shaderFile.rdbuf();
				shaderCode = shaderstream.str();
			}
			const char* shaderCodeChar = shaderCode.c_str();
			switch (type) {
			case VERTEX_SHADER:
				return CompileShader(shaderCodeChar, GL_VERTEX_SHADER);
			case FRAGMENT_SHADER:
				return CompileShader(shaderCodeChar, GL_FRAGMENT_SHADER);
			case GEOMETRY_SHADER:
				return CompileShader(shaderCodeChar, GL_GEOMETRY_SHADER);
			default:
				break;
			}
			return -1;
		}

		int OpenGLShaders::CompileShader(const char* shaderSource, unsigned int shaderType) {
			unsigned int shaderID = glCreateShader(shaderType);
			glShaderSource(shaderID, 1, &shaderSource, NULL);
			glCompileShader(shaderID);
			int success;
			char infoLog[1024];
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
				if (shaderType == GL_VERTEX_SHADER) {
					PV_CORE_ERROR("Shader Compilation Failed of type : GL_VERTEX_SHADER");
				} else if (shaderType == GL_FRAGMENT_SHADER) {
					PV_CORE_ERROR("Shader Compilation Failed of type : GL_FRAGMENT_SHADER");
				} else if (shaderType == GL_GEOMETRY_SHADER) {
					PV_CORE_ERROR("Shader Compilation Failed of type : GL_GEOMETRY_SHADER");
				}
				PV_CORE_ERROR("%s", infoLog);
				PV_CORE_ERROR("-- --------------------------------------------------- --");
				glDeleteShader(shaderID);
				return -1;
			}
			return shaderID;
		}

	}

}
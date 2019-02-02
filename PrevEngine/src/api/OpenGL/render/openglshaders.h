#pragma once

#include "api/shader.h"

namespace prev { namespace opengl {

	class OpenGLShaders : public Shader {
	public:
		OpenGLShaders(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile = nullptr);
		~OpenGLShaders();
	private:
		virtual void UseShader() const override;
		virtual bool ReadShader(std::string vShaderPath, std::string fShaderPath, std::string gShaderPath) override;
		int ReadShaderFile(std::string shaderPath, ShaderTypes type);
		int CompileShader(const char* shaderSource, unsigned int shaderType);
		bool CreateProgram(std::initializer_list<int> shaders);
	private:
		unsigned int m_ProgramID;
	};

} }
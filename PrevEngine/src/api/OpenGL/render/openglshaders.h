#pragma once

#include <glad/glad.h>
#include "api/shader.h"

namespace prev { namespace opengl {

	class OpenGLShaders : public Shader {
	public:
		OpenGLShaders(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile = nullptr);
		~OpenGLShaders();
	private:
		virtual int GetUniformLocation(const char * uniformName) const override;
		virtual void LoadUniform(const glm::mat4 & matrix, int uniformLocation) const override;
		virtual void LoadUniform(const glm::vec3 & data, int uniformLocation) const override;
		virtual void LoadUniform(const glm::vec4 & data, int uniformLocation) const override;
		virtual void LoadUniform(int data, int uniformLocation) const override;
		virtual void UseShader() const override;
		virtual bool ReadShader(std::string vShaderPath, std::string fShaderPath, std::string gShaderPath) override;
		int ReadShaderFile(std::string shaderPath, ShaderTypes type);
		int CompileShader(const char* shaderSource, unsigned int shaderType);
		bool CreateProgram(std::initializer_list<int> shaders);
	private:
		GLuint m_ProgramID;
	};

} }
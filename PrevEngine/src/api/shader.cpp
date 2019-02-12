#include "pch.h"
#include "shader.h"

namespace prev {
	
	Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile) {
		m_VertexShaderFile = vertexShaderFile;
		m_FragmentShaderFile = fragmentShaderFile;
		if(geometryShaderFile != nullptr)
			m_GeometryShaderFile = geometryShaderFile;
		else
			m_GeometryShaderFile = "";
	}

	void Shader::LoadShader()  {
		isComplete = ReadShader(m_VertexShaderFile, m_FragmentShaderFile, m_GeometryShaderFile);
		// TODO Add imgui assert so as to check the console for errors before quitting the engine
		if (isComplete) {
			UseShader();
			m_ModelMatrixLocation = GetUniformLocation("model");
			m_ProjectionLocation = GetUniformLocation("projection");
		}
	}

	void Shader::LoadProjectionMatrix(const glm::mat4 & matrix) const {
		LoadUniform(matrix, m_ProjectionLocation);
	}

	void Shader::LoadModelMatrix(const glm::mat4 & matrix) const {
		LoadUniform(matrix, m_ModelMatrixLocation);
	}

}
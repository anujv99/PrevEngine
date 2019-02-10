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
	}

}
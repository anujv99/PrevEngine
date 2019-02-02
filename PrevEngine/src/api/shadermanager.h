#pragma once

#include "shader.h"

namespace prev {

	class ShaderManager {
	public:
		static const Shader * LoadShader(const char* shaderName, const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile = nullptr);
		static void ReleaseShaders();
	private:
		static std::map<std::string, Shader *> s_Shaders;
	};

}
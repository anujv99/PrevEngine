#pragma once

#include "shader.h"

namespace prev {

	class ShaderManager {
	public:
		static const Shader * LoadShader(const char* shaderName, const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile = nullptr);
		static void ReleaseShaders();
		static void ActivateShader(const Shader * shader);
		static void ActivateShader(std::string shaderName);
		inline static const Shader * GetActiveShader() { PV_ASSERT(s_ActiveShader, "No active shader found"); return s_ActiveShader; }
	private:
		static std::map<std::string, Shader *> s_Shaders;
		static const Shader * s_ActiveShader;
	};

}
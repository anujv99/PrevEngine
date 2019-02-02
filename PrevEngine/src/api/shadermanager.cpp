#include "pch.h"
#include "shadermanager.h"

namespace prev {
	
	std::map<std::string, Shader *> ShaderManager::s_Shaders;
	
	const Shader * ShaderManager::LoadShader(const char* shaderName, const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile) {
		Shader* shader = Shader::Create(vertexShaderFile, fragmentShaderFile, geometryShaderFile);
		if (!shader->IsShaderComplete()) {
			delete shader;
			PV_CORE_CRITICAL("Unable to load shader : %s", shaderName);
			return nullptr;
		}
		std::string stdshaderName = shaderName;
		s_Shaders.insert(std::pair<std::string, Shader *>(stdshaderName, shader));
		PV_CORE_TRACE("Shader successfully created : %s", shaderName);
		return s_Shaders[stdshaderName];
	}

	void ShaderManager::ReleaseShaders() {
		for (auto shader : s_Shaders) {
			delete shader.second;
		}
		s_Shaders.clear();
	}

}
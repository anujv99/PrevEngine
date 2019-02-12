#include "pch.h"
#include "shadermanager.h"

namespace prev {
	
	std::map<std::string, Shader *> ShaderManager::s_Shaders;
	const Shader * ShaderManager::s_ActiveShader = nullptr;
	
	const Shader * ShaderManager::LoadShader(const char* shaderName, const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile) {
		if (s_Shaders.find(shaderName) != s_Shaders.end()) {
			PV_ASSERT(false, "Shader with name (%s) already exists", shaderName);
			return nullptr;
		}
		Shader* shader = Shader::Create(vertexShaderFile, fragmentShaderFile, geometryShaderFile);
		if (!shader->IsShaderComplete()) {
			delete shader;
			PV_ASSERT(false, "Unable to load shader : %s", shaderName);
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

	void ShaderManager::ActivateShader(const Shader * shader) {
		PV_ASSERT(shader, "Invalid shader");
		shader->UseShader();
		s_ActiveShader = shader;
	}

	void ShaderManager::ActivateShader(std::string shaderName) {
		if (s_Shaders.find(shaderName) != s_Shaders.end()) {
			auto shader = s_Shaders[shaderName];
			shader->UseShader();
			s_ActiveShader = shader;
		} else {
			PV_ASSERT(false, "Shader with name %s not found", shaderName.c_str());
		}
	}

}
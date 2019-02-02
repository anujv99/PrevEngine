#pragma once

namespace prev {

	enum ShaderTypes {
		VERTEX_SHADER	= BIT(0),
		FRAGMENT_SHADER	= BIT(1),
		GEOMETRY_SHADER	= BIT(2)
	};

	class Shader {
		friend class ShaderManager;
	public:
		inline bool IsShaderComplete() const { return isComplete; }

		virtual void UseShader() const = 0;
		virtual ~Shader() {}
	protected:
		Shader(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile = nullptr);

		void LoadShader();
		virtual bool ReadShader(std::string vShaderPath, std::string fShaderPath, std::string gShaderPath) = 0;
	protected:
		std::string m_VertexShaderFile, m_FragmentShaderFile, m_GeometryShaderFile;
		bool isComplete = false;
		int m_AttachedShaders = 0;
	private:
		static Shader* Create(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile = nullptr);
	};

}
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
	public:
		virtual int GetUniformLocation(const char * uniformName) const = 0;
		virtual void LoadUniform(const glm::mat4 & matrix, int uniformLocation) const = 0;
		virtual void LoadUniform(const glm::vec3 & data, int uniformLocation) const = 0;
		virtual void LoadUniform(const glm::vec4 & data, int uniformLocation) const = 0;
		virtual void LoadUniform(int data, int uniformLocation) const = 0;

		void LoadProjectionMatrix(const glm::mat4 & matrix) const;
		void LoadModelMatrix(const glm::mat4 & matrix) const;
	protected:
		Shader(const char * vertexShaderFile, const char* fragmentShaderFile, const char * geometryShaderFile = nullptr);

		void LoadShader();
		virtual bool ReadShader(std::string vShaderPath, std::string fShaderPath, std::string gShaderPath) = 0;
	protected:
		std::string m_VertexShaderFile, m_FragmentShaderFile, m_GeometryShaderFile;
		bool isComplete = false;
		int m_AttachedShaders = 0;
		int m_ModelMatrixLocation, m_ProjectionLocation;
	private:
		static Shader* Create(const char * vertexShaderFile, const char * fragmentShaderFile, const char * geometryShaderFile = nullptr);
	};

}
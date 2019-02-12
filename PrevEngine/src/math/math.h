#pragma once

namespace prev {

	class Math {
		friend class Application;
	public:
		inline static glm::mat4 GetProjectionMatrix() { return m_Projection; }
		inline static const glm::mat4 * GetProjectionMatrixPointer() { return &m_Projection; }
		inline static const float GetWorldScale() { return m_WorldScale; }
		inline static void CreateModelMatrix(glm::mat4 & model, glm::vec2 & position, glm::vec2 & scale, float & rotation) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(position, 0.0f));
			model = glm::scale(model, glm::vec3(scale, 1.0f));
			model = glm::rotate(model, rotation, glm::vec3(0, 0, 1));
		}
	private:
		static void Init(float worldScale = 1.0f);
		static void CreateProjectionMatrix();
		static void OnEvent(Event &e);
		static bool WindowResized(WindowResizeEvent &e);
	private:
		static glm::mat4 m_Projection;
		static float m_WorldScale, m_NearPlane, m_FarPlane;
		static float m_WindowWidth, m_WindowHeight;
	};

}
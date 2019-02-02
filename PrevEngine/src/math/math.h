#pragma once

namespace prev {

	class Math {
		friend class Application;
	public:
		inline static glm::mat4 GetProjectionMatrix() { return m_Projection; }
		inline static const glm::mat4 * GetProjectionMatrixPointer() { return &m_Projection; }
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
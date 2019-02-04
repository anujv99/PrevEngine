#include "pch.h"
#include "math.h"

#include "application.h"

namespace prev {

	float Math::m_WorldScale	=  1.0f;
	float Math::m_FarPlane		= -1.0f;
	float Math::m_NearPlane		=  1.0f;

	float Math::m_WindowWidth;
	float Math::m_WindowHeight;

	glm::mat4 Math::m_Projection = glm::mat4(1.0f);

	void Math::Init(float worldScale) {
		m_WorldScale = worldScale;
		m_WindowWidth = Application::GetApplicationInstance()->GetWindow().GetWidth();
		m_WindowHeight = Application::GetApplicationInstance()->GetWindow().GetHeight();
		CreateProjectionMatrix();
	}

	void Math::CreateProjectionMatrix() {
		m_Projection = glm::ortho(0.0f, m_WindowWidth * m_WorldScale,	 // left, right
								  0.0f, m_WindowHeight * m_WorldScale,  // bottom, top
								  m_NearPlane, m_FarPlane);            // near, far
	}

	void Math::OnEvent(Event &e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(Math::WindowResized);
	}

	bool Math::WindowResized(WindowResizeEvent &e) {
		m_WindowWidth = e.GetWidth();
		m_WindowHeight = e.GetHeight();
		//CreateProjectionMatrix();
		return false;
	}

}
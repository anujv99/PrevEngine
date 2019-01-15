#include "pch.h"
#include "opengl.h"

#ifdef PV_RENDERING_API_OPENGL

#include <glad/glad.h>

namespace prev { 

#define BIND_EVENT_FUNC(x) std::bind(&OpenGL::x, this, std::placeholders::_1)

	API * API::Create(unsigned int windowWidth, unsigned int windowHeight) {
		return new graphicsapi::opengl::OpenGL(windowWidth, windowHeight);
	}

	namespace graphicsapi { namespace opengl {

		OpenGL::OpenGL(unsigned int windowWidth, unsigned int windowHeight) : API(windowWidth, windowHeight) {
			SetViewport();
		}

		OpenGL::~OpenGL() {}

		void OpenGL::OnUpdate() {
			glClearColor(0, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void OpenGL::OnEvent(EventDispatcher & dispatcher) {
			dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(WindowSizeCallback));
		}

		void OpenGL::SetViewport() {
			glViewport(0, 0, m_WindowWidth, m_WindowHeight);
		}

	} }
}

#endif
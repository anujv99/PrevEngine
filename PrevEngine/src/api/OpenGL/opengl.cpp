#include "pch.h"
#include "opengl.h"

#ifdef PV_RENDERING_API_OPENGL

#include "openglobjectsmanager.h"
#include "openglrenderer.h"
#include "application.h"

namespace prev {

	API * API::Create() {
		unsigned int windowWidth = Application::GetApplicationInstance()->GetWindow().GetWidth();
		unsigned int windowHeight = Application::GetApplicationInstance()->GetWindow().GetHeight();
		return new opengl::OpenGL(windowWidth, windowHeight);
	}

	namespace opengl {

		OpenGL::OpenGL(unsigned int windowWidth, unsigned int windowHeight) : API(windowWidth, windowHeight) {
			SetViewport();
			OpenGLRenderer::Init();
		}

		OpenGL::~OpenGL() {
			OpenGLObjectsManager::ReleaseEverything();
		}

		void OpenGL::OnUpdate() {
			glClearColor(0, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void OpenGL::OnEvent(Event &e) {
			EventDispatcher dispatcher(e);
			dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OpenGL::WindowSizeCallback));
		}

		void OpenGL::SetViewport() {
			glViewport(0, 0, m_WindowWidth, m_WindowHeight);
		}

	}

}

#endif
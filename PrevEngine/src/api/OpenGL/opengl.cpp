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

		static void GLAPIENTRY
			MessageCallback(GLenum source,
							GLenum type,
							GLuint id,
							GLenum severity,
							GLsizei length,
							const GLchar * message,
							const void * userParam) {
			fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
				(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
					type, severity, message);
		}

		OpenGL::OpenGL(unsigned int windowWidth, unsigned int windowHeight) : API(windowWidth, windowHeight) {
			SetViewport();
			OpenGLRenderer::Init();

			// During init, enable debug output
			//glEnable(GL_DEBUG_OUTPUT);
			//glDebugMessageCallback(MessageCallback, 0);

		}

		OpenGL::~OpenGL() {
			OpenGLObjectsManager::ReleaseEverything();
		}

		void OpenGL::OnUpdate() {
			glClearColor(0, 0, 0, 0);
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
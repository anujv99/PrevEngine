#include "pch.h"
#include "Application.h"

namespace prev {

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application() {
		m_Window = std::shared_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNC(OnEvent));
		m_Input = std::unique_ptr<Input>(new Input(m_Window));
#ifdef PV_RENDERING_API_OPENGL
		#pragma comment(lib, "opengl32.lib")
		m_Window->CreateOpenGLContext();
#elif PV_RENDERING_API_DIRECTX
		#pragma comment(lib, "dxgi.lib")
		#pragma comment(lib, "d3d11.lib")
		#pragma comment(lib, "d3dcompiler.lib")
		#pragma comment(lib, "winmm.lib")
		m_Window->CreateDirectXContext();
#endif
	}

	Application::~Application() {
	}

	void Application::OnEvent(Event &event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClose));
		
		//PV_CORE_TRACE(event.ToString().c_str());
	}

	void Application::Run() {
		while (m_Running) {
			m_Input->Update();
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent &e) {
		m_Running = false;
		return true;
	}

}
#include "pch.h"
#include "Application.h"

#include "essentials/timer.h"

namespace prev {

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application() {
		m_Window = std::shared_ptr<Window>(Window::Create()); // Create Window based on platform
		m_Window->SetEventCallback(BIND_EVENT_FUNC(OnEvent)); // Set EventCallback
		m_Input = std::shared_ptr<Input>(new Input(m_Window)); // Create Input Class
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
		// Create Graphics Class based on api
		m_GraphicsAPI = std::unique_ptr<API>(API::Create(m_Window->GetWidth(), m_Window->GetHeight()));
		Timer::FPSCounter(true);
	}

	Application::~Application() {
	}

	void Application::OnEvent(Event &event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(event);
			if (event.Handled())
				break;
		}

		if (event.IsInCategory(EventCategoryMouse)) {
			m_Input->OnEvent(dispatcher);
		}

		if (event.IsInCategory(EventCategoryApplication)) {
			m_GraphicsAPI->OnEvent(dispatcher);
		}
	}

	void Application::PushLayer(Layer * layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer * layer) {
		m_LayerStack.PushOverlay(layer);
	}

	void Application::Run() {
		while (m_Running) {
			Timer::Update();
			m_GraphicsAPI->OnUpdate();

			for (Layer * layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_Input->OnUpdate();
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent &e) {
		m_Running = false;
		return true;
	}

}
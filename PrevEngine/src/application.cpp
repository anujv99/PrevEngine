#include "pch.h"
#include "application.h"
#include "essentials/timer.h"
#include "imgui/imguilayer.h"

namespace prev {

	static Application * s_Instance = nullptr;

	Application::Application() {
		s_Instance = this; //Do this first
		PV_CORE_INFO("Engine Starting UP!");
		m_Window = std::shared_ptr<Window>(Window::Create());				  // Create Window based on platform
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));	 // Set EventCallback
		m_Input = std::shared_ptr<Input>(new Input(m_Window));				// Create Input Class
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
		PushLayer(new ImGuiLayer());
	}

	Application::~Application() {
	}

	void Application::OnEvent(Event &event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(event);
			if (event.Handled())
				break;
		}

		m_Input->OnEvent(event);
		m_GraphicsAPI->OnEvent(event);
	}

	void Application::PushLayer(Layer * layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer * layer) {
		m_LayerStack.PushOverlay(layer);
	}

	Application * Application::GetApplicationInstance() {
		return s_Instance;
	}

	void Application::Run() {
		while (m_Running) {
			Timer::Update();
			m_GraphicsAPI->OnUpdate();

			for (Layer * layer : m_LayerStack) {
				layer->OnUpdate();
			}

			if (Input::IsKeyDown(keyboard::PV_KEYBOARD_KEY_C)) {
				PV_CORE_CRITICAL("C Pressed");
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
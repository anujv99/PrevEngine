 #include "pch.h"
#include "application.h"
#include "essentials/timer.h"
#include "imgui/imguilayer.h"
#include "math/math.h"

#include "math/tiles.h"

namespace prev {

	static Application * s_Instance = nullptr;

	Application::Application() {
		s_Instance = this; //Do this first
		PV_CORE_INFO("Engine Starting UP!");
		m_Window = std::shared_ptr<Window>(Window::Create());				  // Create Window based on platform
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));	 // Set EventCallback
		m_Input = std::shared_ptr<Input>(new Input(m_Window));				// Create Input Class
		Math::Init(0.01f);
#ifdef PV_RENDERING_API_OPENGL
		#pragma comment(lib, "opengl32.lib")
		m_Window->CreateOpenGLContext();
#else
		#error Define PV_RENDERING_API_OPENGL as DirectX is currently not supported
#endif
		// Create Graphics Class based on api
		PushLayer(API::Create());
		PushOverlay(new ImGuiLayer());
	}

	Application::~Application() {
	}

	void Application::OnEvent(Event &event) {
		Math::OnEvent(event);
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		m_LayerStack.OnEvent(event);
		m_Input->OnEvent(event);
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

			m_LayerStack.OnUpdate();

			m_Input->OnUpdate();
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent &e) {
		m_Running = false;
		return true;
	}

}
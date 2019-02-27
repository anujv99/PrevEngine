 #include "pch.h"
#include "application.h"
#include "essentials/timer.h"
#include "imgui/imguilayer.h"
#include "math/math.h"

#include "math/tiles.h"
#include "game/level/levelmanager.h"

#include "collision/box2ddebuglayer.h"
#include "lua/luascript.h"

namespace prev {

	static Application * s_Instance = nullptr;
	Box2DDebugLayer * b2DebugDraw;

	Application::Application(WindowProps winProps) {
		s_Instance = this; //Do this first
		PV_CORE_INFO("Engine Starting UP!");
		m_Window = std::shared_ptr<Window>(Window::Create(winProps));		// Create Window based on platform
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));   // Set EventCallback
		m_Input = std::shared_ptr<Input>(new Input(m_Window));			  // Create Input Class
		Math::Init(0.01f);
#ifdef PV_RENDERING_API_OPENGL
		#pragma comment(lib, "opengl32.lib")
		m_Window->CreateOpenGLContext();
#else
		#error Define PV_RENDERING_API_OPENGL as DirectX is currently not supported
#endif
		// Create Graphics Class based on api
		PushLayer(API::Create());
		b2DebugDraw = new Box2DDebugLayer();
		PushOverlay(b2DebugDraw);
		//PushOverlay(new ImGuiLayer());

		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;
		flags += b2Draw::e_aabbBit;
		flags += b2Draw::e_pairBit;
		flags += b2Draw::e_centerOfMassBit;
		b2DebugDraw->Getb2Draw()->SetFlags(flags);

		LevelManager::Init();
	}

	Application::~Application() {
		LevelManager::Release();
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

	void Application::PopLayer(Layer * layer) {
		m_LayerStack.PopLayer(layer);
	}

	void Application::PopOverlay(Layer * layer) {
		m_LayerStack.PopOverlay(layer);
	}

	b2Draw * Application::GetB2Draw() {
		return b2DebugDraw->Getb2Draw();
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
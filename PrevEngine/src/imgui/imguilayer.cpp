#include "pch.h"
#include "imguilayer.h"
#include "essentials/timer.h"
#include "essentials/input.h"
#include "application.h"

#include "imguilogger.h"

namespace prev {

	static bool is_demo = true;
	static bool is_logging = true;

	ImGuiLayer::ImGuiLayer() {
		m_WindowWidth = Application::GetApplicationInstance()->GetWindow().GetWidth();
		m_WindowHeight = Application::GetApplicationInstance()->GetWindow().GetHeight();

		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO &io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		m_API = std::unique_ptr<ImGuiWrapper>(ImGuiWrapper::Initialize());
		m_API->Init();

		{
			io.KeyMap[ImGuiKey_Tab]			= keyboard::PV_KEYBOARD_KEY_TAB;
			io.KeyMap[ImGuiKey_LeftArrow]	= keyboard::PV_KEYBOARD_KEY_LEFT;
			io.KeyMap[ImGuiKey_RightArrow]	= keyboard::PV_KEYBOARD_KEY_RIGHT;
			io.KeyMap[ImGuiKey_UpArrow]		= keyboard::PV_KEYBOARD_KEY_UP;
			io.KeyMap[ImGuiKey_DownArrow]	= keyboard::PV_KEYBOARD_KEY_DOWN;
			io.KeyMap[ImGuiKey_PageUp]		= keyboard::PV_KEYBOARD_KEY_PAGEUP;
			io.KeyMap[ImGuiKey_PageDown]	= keyboard::PV_KEYBOARD_KEY_PAGEDOWN;
			io.KeyMap[ImGuiKey_Home]		= keyboard::PV_KEYBOARD_KEY_HOME;
			io.KeyMap[ImGuiKey_End]			= keyboard::PV_KEYBOARD_KEY_END;
			io.KeyMap[ImGuiKey_Insert]		= keyboard::PV_KEYBOARD_KEY_INSERT;
			io.KeyMap[ImGuiKey_Delete]		= keyboard::PV_KEYBOARD_KEY_DELETE;
			io.KeyMap[ImGuiKey_Backspace]	= keyboard::PV_KEYBOARD_KEY_BACKSPACE;
			io.KeyMap[ImGuiKey_Space]		= keyboard::PV_KEYBOARD_KEY_SPACE;
			io.KeyMap[ImGuiKey_Enter]		= keyboard::PV_KEYBOARD_KEY_ENTER;
			io.KeyMap[ImGuiKey_Escape]		= keyboard::PV_KEYBOARD_KEY_ESCAPE;
			io.KeyMap[ImGuiKey_A]			= keyboard::PV_KEYBOARD_KEY_A;
			io.KeyMap[ImGuiKey_C]			= keyboard::PV_KEYBOARD_KEY_C;
			io.KeyMap[ImGuiKey_V]			= keyboard::PV_KEYBOARD_KEY_V;
			io.KeyMap[ImGuiKey_X]			= keyboard::PV_KEYBOARD_KEY_X;
			io.KeyMap[ImGuiKey_Y]			= keyboard::PV_KEYBOARD_KEY_Y;
			io.KeyMap[ImGuiKey_Z]			= keyboard::PV_KEYBOARD_KEY_Z;
		}
		{
			m_ImGuiMouseCursorMap[ImGuiMouseCursor_Arrow]		= CursorType::PV_ARROW;
			m_ImGuiMouseCursorMap[ImGuiMouseCursor_TextInput]	= CursorType::PV_TEXT_INPUT;
			m_ImGuiMouseCursorMap[ImGuiMouseCursor_ResizeAll]	= CursorType::PV_RESIZE_ALL;
			m_ImGuiMouseCursorMap[ImGuiMouseCursor_ResizeEW]	= CursorType::PV_RESIZE_EW;
			m_ImGuiMouseCursorMap[ImGuiMouseCursor_ResizeNS]	= CursorType::PV_RESIZE_NS;
			m_ImGuiMouseCursorMap[ImGuiMouseCursor_ResizeNESW]	= CursorType::PV_RESIZE_NESW;
			m_ImGuiMouseCursorMap[ImGuiMouseCursor_ResizeNWSE]	= CursorType::PV_RESIZE_NWSE;
			m_ImGuiMouseCursorMap[ImGuiMouseCursor_Hand]		= CursorType::PV_HAND;
		}
		{
			m_LogColors[LogLevel::PV_INFO]		= ImVec4(0, 1, 0, 1);
			m_LogColors[LogLevel::PV_TRACE]		= ImVec4(0, 1, 1, 1);
			m_LogColors[LogLevel::PV_WARN]		= ImVec4(1, 1, 0, 1);
			m_LogColors[LogLevel::PV_ERROR]		= ImVec4(1, 0, 0, 1);
			m_LogColors[LogLevel::PV_CRITICAL]	= ImVec4(1, 0, 1, 1);
		}

	}

	ImGuiLayer::~ImGuiLayer() {
	}

	void ImGuiLayer::OnAttach() {
		PV_CORE_INFO("ImGui Layer attached!");
	}

	void ImGuiLayer::OnDetach() {
		PV_CORE_INFO("ImGui Layer detached!");
	}

	void ImGuiLayer::OnUpdate() {
		ImGuiIO &io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)m_WindowWidth, (float)m_WindowHeight);
		io.DeltaTime = Timer::GetDeltaTime();

		UpdateMouseCursor();

		m_API->NewFrame();
		ImGui::NewFrame();

		if (is_demo)
			ImGui::ShowDemoWindow(&is_demo);
		if (Input::IsKeyDown(keyboard::PV_KEYBOARD_KEY_P)) {
			is_demo = true;
		}

		if (is_logging)
			ShowLogger();

		ImGui::EndFrame();
		ImGui::Render();
		m_API->Render(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event &event) {
		ImGuiIO &io = ImGui::GetIO();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::MouseMoved));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::MouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::MouseButtonReleased));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiLayer::MouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::WindowResized));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::KeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::KeyReleased));

		if (event.GetEventType() == EventType::CharacterInput) {
			CharacterEvent &e = *((CharacterEvent*)(&event));
			io.AddInputCharacter(e.GetPressedChar());
		}
	}

	void ImGuiLayer::ShowLogger() {
		static ImGuiAppLog log;

		Log::SetLoggerCallbackFunction([this](std::string s, LogLevel l)->void {
			log.AddLog(m_LogColors[l], s);
		});

		log.Draw("Log", &is_logging);
	}

	void ImGuiLayer::UpdateMouseCursor() {
		ImGuiMouseCursor cursor = ImGui::GetMouseCursor();
		Application::GetApplicationInstance()->GetWindow().ChangeCursor(m_ImGuiMouseCursorMap[cursor]);
	}
	bool ImGuiLayer::MouseMoved(MouseMovedEvent &e) {
		ImGuiIO &io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		return false;
	}
	bool ImGuiLayer::MouseButtonPressed(MouseButtonPressedEvent &e) {
		ImGuiIO &io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;
		return false;
	}
	bool ImGuiLayer::MouseButtonReleased(MouseButtonReleasedEvent &e) {
		ImGuiIO &io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;
		return false;
	}
	bool ImGuiLayer::MouseScrolled(MouseScrolledEvent &e) {
		ImGuiIO &io = ImGui::GetIO();
		io.MouseWheelH = e.GetYOffset();
		io.MouseWheel = e.GetXOffset();
		return false;
	}
	bool ImGuiLayer::WindowResized(WindowResizeEvent &e) {
		m_WindowWidth = e.GetWidth();
		m_WindowHeight = e.GetHeight();
		return false;
	}
	bool ImGuiLayer::KeyPressed(KeyPressedEvent &e) {
		ImGuiIO &io = ImGui::GetIO();
		if (!e.IsRepeating()) {
			io.KeysDown[e.GetKeyCode()] = true;
		}
		return false;
	}
	bool ImGuiLayer::KeyReleased(KeyReleasedEvent &e) {
		ImGuiIO &io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;
		return false;
	}



}
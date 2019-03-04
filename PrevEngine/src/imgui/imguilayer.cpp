#include "pch.h"
#include "imguilayer.h"
#include "essentials/timer.h"
#include "essentials/input.h"
#include "application.h"

#include "imguilogger.h"

namespace prev {

	static bool is_demo				= true;
	static bool is_logging			= true;
	static bool is_miscPropsWindow	= true;
	static bool changeLogPorps		= false;
	static ImGuiAppLog log;
	static ImGuiMouseCursor lastCursor;

	ImGuiLayer::ImGuiLayer() {
		m_WindowWidth = Application::GetApplicationInstance()->GetWindow().GetWidth();
		m_WindowHeight = Application::GetApplicationInstance()->GetWindow().GetHeight();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO & io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();

		ImGuiStyle & style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		m_API = ImGuiWrapper::Initialize();
		m_API->Init();

		{
			io.KeyMap[ImGuiKey_Tab]			= PV_KEYBOARD_KEY_TAB;
			io.KeyMap[ImGuiKey_LeftArrow]	= PV_KEYBOARD_KEY_LEFT;
			io.KeyMap[ImGuiKey_RightArrow]	= PV_KEYBOARD_KEY_RIGHT;
			io.KeyMap[ImGuiKey_UpArrow]		= PV_KEYBOARD_KEY_UP;
			io.KeyMap[ImGuiKey_DownArrow]	= PV_KEYBOARD_KEY_DOWN;
			io.KeyMap[ImGuiKey_PageUp]		= PV_KEYBOARD_KEY_PAGEUP;
			io.KeyMap[ImGuiKey_PageDown]	= PV_KEYBOARD_KEY_PAGEDOWN;
			io.KeyMap[ImGuiKey_Home]		= PV_KEYBOARD_KEY_HOME;
			io.KeyMap[ImGuiKey_End]			= PV_KEYBOARD_KEY_END;
			io.KeyMap[ImGuiKey_Insert]		= PV_KEYBOARD_KEY_INSERT;
			io.KeyMap[ImGuiKey_Delete]		= PV_KEYBOARD_KEY_DELETE;
			io.KeyMap[ImGuiKey_Backspace]	= PV_KEYBOARD_KEY_BACKSPACE;
			io.KeyMap[ImGuiKey_Space]		= PV_KEYBOARD_KEY_SPACE;
			io.KeyMap[ImGuiKey_Enter]		= PV_KEYBOARD_KEY_ENTER;
			io.KeyMap[ImGuiKey_Escape]		= PV_KEYBOARD_KEY_ESCAPE;
			io.KeyMap[ImGuiKey_A]			= PV_KEYBOARD_KEY_A;
			io.KeyMap[ImGuiKey_C]			= PV_KEYBOARD_KEY_C;
			io.KeyMap[ImGuiKey_V]			= PV_KEYBOARD_KEY_V;
			io.KeyMap[ImGuiKey_X]			= PV_KEYBOARD_KEY_X;
			io.KeyMap[ImGuiKey_Y]			= PV_KEYBOARD_KEY_Y;
			io.KeyMap[ImGuiKey_Z]			= PV_KEYBOARD_KEY_Z;
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

		Log::SetLoggerCallbackFunction([this](std::string s, LogLevel l)->void {
			log.AddLog(l, s);
		});

		m_B2DebugLayer = Application::GetApplicationInstance()->GetB2Draw();

	}

	ImGuiLayer::~ImGuiLayer() {
		delete m_API;
		ImGui::DestroyContext();
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

		if (is_logging)
			ShowLogger();

		if (is_miscPropsWindow)
			ShowMiscPropsWindow();

		ShowMainMenuBar();

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
		dispatcher.Dispatch<CharacterEvent>(BIND_EVENT_FN(ImGuiLayer::CharacterInputEvent));
	}

	void ImGuiLayer::ShowMainMenuBar() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Misc")) {
				ImGui::MenuItem("Show Log", NULL, &is_logging);
				ImGui::MenuItem("Show Demo Window", NULL, &is_demo);
				ImGui::MenuItem("Change Properties", NULL, &is_miscPropsWindow);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Log")) {
				ImGui::MenuItem("Show Log", NULL, &is_logging);
				ImGui::MenuItem("Change Log Properties", NULL, &changeLogPorps);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	void ImGuiLayer::ShowMiscPropsWindow() {
		ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Misc Properties", &is_miscPropsWindow)) {

			if (ImGui::CollapsingHeader("Box2D Debug Draw Props")) {
				if (m_B2DebugLayer != nullptr) {
					static bool b2_debug_draw_enabled = true;
					static unsigned int current_flags = 0;
					if (ImGui::Checkbox("Box2D Debug Enabled", &b2_debug_draw_enabled)) {
						unsigned int flags = m_B2DebugLayer->GetFlags();
						m_B2DebugLayer->SetFlags(current_flags);
						current_flags = flags;
					}
					if (b2_debug_draw_enabled) {
						static bool is_shape_bit	= true;
						static bool is_joint_bit	= true;
						static bool is_aabb_bit		= true;
						static bool is_pair_bit		= true;
						static bool is_cm_bit		= true; // Center of mass bit
						if (ImGui::Checkbox("Shape Bit", &is_shape_bit)) {
							if (is_shape_bit) {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() + b2Draw::e_shapeBit);
							} else {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() - b2Draw::e_shapeBit);
							}
						} else if (ImGui::Checkbox("Joint Bit", &is_joint_bit)) {
							if (is_joint_bit) {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() + b2Draw::e_jointBit);
							} else {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() - b2Draw::e_jointBit);
							}
						} else if (ImGui::Checkbox("AABB Bit", &is_aabb_bit)) {
							if (is_aabb_bit) {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() + b2Draw::e_aabbBit);
							} else {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() - b2Draw::e_aabbBit);
							}
						} else if (ImGui::Checkbox("Pait Bit", &is_pair_bit)) {
							if (is_pair_bit) {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() + b2Draw::e_pairBit);
							} else {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() - b2Draw::e_pairBit);
							}
						} else if (ImGui::Checkbox("Center of Mass Bit", &is_cm_bit)) {
							if (is_cm_bit) {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() + b2Draw::e_centerOfMassBit);
							} else {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() - b2Draw::e_centerOfMassBit);
							}
						}
					}
				}
			}

		}
		ImGui::End();
	}

	void ImGuiLayer::ShowLogger() {
		log.Draw("Log", m_LogColors, &is_logging);
		if (changeLogPorps) {
			if (ImGui::Begin("Log Props", &changeLogPorps)) {
				if (ImGui::CollapsingHeader("Log Level Colors")) {
					ImGui::ColorEdit4("Info", (float*)&m_LogColors[LogLevel::PV_INFO]);
					ImGui::ColorEdit4("Trace", (float*)&m_LogColors[LogLevel::PV_TRACE]);
					ImGui::ColorEdit4("Warn", (float*)&m_LogColors[LogLevel::PV_WARN]);
					ImGui::ColorEdit4("Error", (float*)&m_LogColors[LogLevel::PV_ERROR]);
					ImGui::ColorEdit4("Critical", (float*)&m_LogColors[LogLevel::PV_CRITICAL]);
				}
				ImGui::End();
			}
		}
	}

	void ImGuiLayer::UpdateMouseCursor() {
		ImGuiMouseCursor cursor = ImGui::GetMouseCursor();
		if (lastCursor != cursor) {
			lastCursor = cursor;
			Application::GetApplicationInstance()->GetWindow().ChangeCursor(m_ImGuiMouseCursorMap[cursor]);
		}
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

	bool ImGuiLayer::CharacterInputEvent(CharacterEvent &e) {
		ImGuiIO &io = ImGui::GetIO();
		io.AddInputCharacter(e.GetPressedChar());
		return false;
	}

}
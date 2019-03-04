#include "pch.h"
#include "imguiopenglinit.h"

#include <glad/glad.h>
#include <imgui.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_opengl3.cpp"

#include "application.h"

namespace prev {

	static ImGuiMouseCursor lastCursor;

	ImGuiWrapper * ImGuiWrapper::Initialize() {
		return new windows::ImGuiOpenGLInit();
	}

	namespace windows {

		ImGuiOpenGLInit::ImGuiOpenGLInit() : ImGuiWrapper() { }

		ImGuiOpenGLInit::~ImGuiOpenGLInit() {
			ImGui_ImplOpenGL3_Shutdown();
		}

		void ImGuiOpenGLInit::Init() {
			m_WindowWidth = Application::GetApplicationInstance()->GetWindow().GetWidth();
			m_WindowHeight = Application::GetApplicationInstance()->GetWindow().GetHeight();

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO & io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

			ImGui::StyleColorsDark();

			ImGuiStyle & style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			ImGui_ImplOpenGL3_Init("#version 460 core");

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
		}

		void ImGuiOpenGLInit::NewFrame() {
			ImGuiIO & io = ImGui::GetIO();
			io.DisplaySize = ImVec2((float)m_WindowWidth, (float)m_WindowHeight);
			io.DeltaTime = Timer::GetDeltaTime();

			UpdateMouseCursor();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();
		}

		void ImGuiOpenGLInit::Render() {
			ImGui::EndFrame();
			ImGui::Render();
			ImGui::UpdatePlatformWindows();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void ImGuiOpenGLInit::OnEvent(Event & event) {
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiOpenGLInit::MouseMoved));
			dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiOpenGLInit::MouseButtonPressed));
			dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiOpenGLInit::MouseButtonReleased));
			dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiOpenGLInit::MouseScrolled));
			dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiOpenGLInit::WindowResized));
			dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiOpenGLInit::KeyPressed));
			dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiOpenGLInit::KeyReleased));
			dispatcher.Dispatch<CharacterEvent>(BIND_EVENT_FN(ImGuiOpenGLInit::CharacterInputEvent));
		}

		void ImGuiOpenGLInit::UpdateMouseCursor() {
			ImGuiMouseCursor cursor = ImGui::GetMouseCursor();
			if (lastCursor != cursor) {
				lastCursor = cursor;
				Application::GetApplicationInstance()->GetWindow().ChangeCursor(m_ImGuiMouseCursorMap[cursor]);
			}
		}


		bool ImGuiOpenGLInit::MouseMoved(MouseMovedEvent & e) {
			ImGuiIO & io = ImGui::GetIO();
			io.MousePos = ImVec2(e.GetX(), e.GetY());
			return false;
		}
		bool ImGuiOpenGLInit::MouseButtonPressed(MouseButtonPressedEvent & e) {
			ImGuiIO & io = ImGui::GetIO();
			io.MouseDown[e.GetMouseButton()] = true;
			return false;
		}
		bool ImGuiOpenGLInit::MouseButtonReleased(MouseButtonReleasedEvent & e) {
			ImGuiIO & io = ImGui::GetIO();
			io.MouseDown[e.GetMouseButton()] = false;
			return false;
		}
		bool ImGuiOpenGLInit::MouseScrolled(MouseScrolledEvent & e) {
			ImGuiIO & io = ImGui::GetIO();
			io.MouseWheelH = e.GetYOffset();
			io.MouseWheel = e.GetXOffset();
			return false;
		}
		bool ImGuiOpenGLInit::WindowResized(WindowResizeEvent & e) {
			m_WindowWidth = e.GetWidth();
			m_WindowHeight = e.GetHeight();
			return false;
		}
		bool ImGuiOpenGLInit::KeyPressed(KeyPressedEvent & e) {
			ImGuiIO & io = ImGui::GetIO();
			if (!e.IsRepeating()) {
				io.KeysDown[e.GetKeyCode()] = true;
			}
			return false;
		}
		bool ImGuiOpenGLInit::KeyReleased(KeyReleasedEvent & e) {
			ImGuiIO & io = ImGui::GetIO();
			io.KeysDown[e.GetKeyCode()] = false;
			return false;
		}

		bool ImGuiOpenGLInit::CharacterInputEvent(CharacterEvent & e) {
			ImGuiIO & io = ImGui::GetIO();
			io.AddInputCharacter(e.GetPressedChar());
			return false;
		}

	}

}

#include "pch.h"
#include "windowswindow.h"

#include "engine/events/applicationevent.h"
#include "engine/events/keyevent.h"
#include "engine/events/mouseevent.h"

#include "misc.h"
#include "API/OpenGL/openglwindowsinitialization.h"

#include "essentials/input.h"

#define MOUSE_SCROLL_FACTOR 0.01f

namespace prev {

	std::string ws2s(const std::wstring & ws) {
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(ws);
	}

	//For std::string to std::wstring
	std::wstring s2ws(const std::string & s) {
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(s);
	}

	std::string Window::m_ExePath;

	Window * Window::Create(const WindowProps &props) {
		return new windows::WindowsWindow(props);
	}

	std::string Window::GetExePath() {
		if (m_ExePath.size() != 0) {
			return m_ExePath;
		}
		HMODULE hModule = GetModuleHandle(NULL);
		WCHAR path[MAX_PATH];
		GetModuleFileName(hModule, path, MAX_PATH);
		std::string spath = ws2s(std::wstring(path));
		return spath.substr(0, spath.find_last_of("\\"));
	}

	namespace windows {

		static auto s_WindowClassName = L"WindowClassPrevEngine";
		static WindowsWindow * s_WindowPointer = nullptr;

		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			switch(msg) {
			case WM_CREATE:
				PV_CORE_INFO("Windows window created");
				break;
			case WM_DESTROY:
				break;
			case WM_CLOSE:
			{
				WindowCloseEvent event;
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_MOVE:
			{
				if(s_WindowPointer->m_Data.eventCallback == NULL) break;
				WindowMoveEvent event((unsigned int)(short)LOWORD(lParam), (unsigned int)(short)HIWORD(lParam));
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_SIZE:
			{
				if(s_WindowPointer->m_Data.eventCallback == NULL) break;
				WindowResizeEvent event(LOWORD(lParam), HIWORD(lParam));
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_CHAR:
			{
				CharacterEvent event((char)wParam);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_KEYDOWN:
			{
				try {
					int keyCode = s_WindowPointer->m_ReverseKeyMap.at(wParam);
					bool repeatCount = (lParam & 0x40000000);
					KeyPressedEvent event(keyCode, repeatCount);
					s_WindowPointer->m_Data.eventCallback(event);
				} catch(std::exception &e) {}
				break;
			}
			case WM_KEYUP:
			{
				try {
					int keyCode = s_WindowPointer->m_ReverseKeyMap.at(wParam);
					KeyReleasedEvent event(keyCode);
					s_WindowPointer->m_Data.eventCallback(event);
				} catch(std::exception &e) {}
				break;
			}
			case WM_LBUTTONDOWN:
			{
				MouseButtonPressedEvent event(mouse::PV_MOUSE_BUTTON_LEFT);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_RBUTTONDOWN:
			{
				MouseButtonPressedEvent event(mouse::PV_MOUSE_BUTTON_RIGHT);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_MBUTTONDOWN:
			{
				MouseButtonPressedEvent event(mouse::PV_MOUSE_BUTTON_MIDDLE);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_XBUTTONDOWN:
			{
				unsigned short button = GET_XBUTTON_WPARAM(wParam);
				if(button == 1) {
					MouseButtonPressedEvent event(mouse::PV_MOUSE_BUTTON_THUMB1);
					s_WindowPointer->m_Data.eventCallback(event);
					break;
				} else if(button == 2) {
					MouseButtonPressedEvent event(mouse::PV_MOUSE_BUTTON_THUMB2);
					s_WindowPointer->m_Data.eventCallback(event);
					break;
				}
			}
			case WM_LBUTTONUP:
			{
				MouseButtonReleasedEvent event(mouse::PV_MOUSE_BUTTON_LEFT);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_RBUTTONUP:
			{
				MouseButtonReleasedEvent event(mouse::PV_MOUSE_BUTTON_RIGHT);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_MBUTTONUP:
			{
				MouseButtonReleasedEvent event(mouse::PV_MOUSE_BUTTON_MIDDLE);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_XBUTTONUP:
			{
				unsigned short button = GET_XBUTTON_WPARAM(wParam);
				if(button == 1) {
					MouseButtonReleasedEvent event(mouse::PV_MOUSE_BUTTON_THUMB1);
					s_WindowPointer->m_Data.eventCallback(event);
					break;
				} else if(button == 2) {
					MouseButtonReleasedEvent event(mouse::PV_MOUSE_BUTTON_THUMB2);
					s_WindowPointer->m_Data.eventCallback(event);
					break;
				}
			}
			case WM_MOUSEMOVE:
			{
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);
				MouseMovedEvent event((float)xPos, (float)yPos);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_MOUSEWHEEL:
			{
				static float deltaScroll = 0.0f;
				float xPos = GET_WHEEL_DELTA_WPARAM(wParam);
				MouseScrolledEvent event(xPos * MOUSE_SCROLL_FACTOR, 0);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
			}
			return 0;
		}

		WindowsWindow::WindowsWindow(const WindowProps &props) {
			PV_CORE_TRACE("Creating Windows window [Title = %s] [Width = %d] [Height = %d]", props.Title.c_str(), props.Width, props.Height);
			m_Data.title = props.Title;
			m_Data.width = props.Width;
			m_Data.height = props.Height;
			m_Data.cType = props.CType; // C = cursor

			WNDCLASSEX wc = {0};
			HINSTANCE hInstance = 0;

			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = 0;
			wc.lpfnWndProc = WndProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = s_WindowClassName;
			wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
			s_WindowPointer = this;

			if(!RegisterClassEx(&wc)) {
				PV_CORE_ERROR("Unable to register window class");
				return;
			}

			RECT rect = {0, 0, props.Width, props.Height };
			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

			hWnd = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				s_WindowClassName,
				s2ws(props.Title).c_str(),
				WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
				CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
				NULL, NULL, hInstance, NULL);

			if(hWnd == NULL) {
				PV_CORE_ERROR("Unable to create Windows Window");
				return;
			}

			ShowWindow(hWnd, SW_SHOWNORMAL);
			UpdateWindow(hWnd);

			{
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_A] = 0x41;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_B] = 0x42;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_C] = 0x43;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_D] = 0x44;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_E] = 0x45;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_F] = 0x46;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_G] = 0x47;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_H] = 0x48;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_I] = 0x49;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_J] = 0x4A;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_K] = 0x4B;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_L] = 0x4C;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_M] = 0x4D;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_N] = 0x4E;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_O] = 0x4F;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_P] = 0x50;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_Q] = 0x51;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_R] = 0x52;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_S] = 0x53;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_T] = 0x54;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_U] = 0x55;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_V] = 0x56;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_W] = 0x57;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_X] = 0x58;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_Y] = 0x59;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_Z] = 0x5A;

				m_KeyMap[keyboard::PV_KEYBOARD_KEY_LEFT]		= VK_LEFT;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_RIGHT]		= VK_RIGHT;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_UP]			= VK_UP;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_DOWN]		= VK_DOWN;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_ESCAPE]		= VK_ESCAPE;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_TAB]			= VK_TAB;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_INSERT]		= VK_INSERT;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_HOME]		= VK_HOME;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_PAGEUP]		= VK_PRIOR;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_PAGEDOWN]	= VK_NEXT;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_END]			= VK_END;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_DELETE]		= VK_DELETE;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_BACKSPACE]	= VK_BACK;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_SPACE]		= VK_SPACE;
				m_KeyMap[keyboard::PV_KEYBOARD_KEY_ENTER]		= VK_RETURN;
			}
			{
				m_ReverseKeyMap[0x41] = keyboard::PV_KEYBOARD_KEY_A;
				m_ReverseKeyMap[0x42] = keyboard::PV_KEYBOARD_KEY_B;
				m_ReverseKeyMap[0x43] = keyboard::PV_KEYBOARD_KEY_C;
				m_ReverseKeyMap[0x44] = keyboard::PV_KEYBOARD_KEY_D;
				m_ReverseKeyMap[0x45] = keyboard::PV_KEYBOARD_KEY_E;
				m_ReverseKeyMap[0x46] = keyboard::PV_KEYBOARD_KEY_F;
				m_ReverseKeyMap[0x47] = keyboard::PV_KEYBOARD_KEY_G;
				m_ReverseKeyMap[0x48] = keyboard::PV_KEYBOARD_KEY_H;
				m_ReverseKeyMap[0x49] = keyboard::PV_KEYBOARD_KEY_I;
				m_ReverseKeyMap[0x4A] = keyboard::PV_KEYBOARD_KEY_J;
				m_ReverseKeyMap[0x4B] = keyboard::PV_KEYBOARD_KEY_K;
				m_ReverseKeyMap[0x4C] = keyboard::PV_KEYBOARD_KEY_L;
				m_ReverseKeyMap[0x4D] = keyboard::PV_KEYBOARD_KEY_M;
				m_ReverseKeyMap[0x4E] = keyboard::PV_KEYBOARD_KEY_N;
				m_ReverseKeyMap[0x4F] = keyboard::PV_KEYBOARD_KEY_O;
				m_ReverseKeyMap[0x50] = keyboard::PV_KEYBOARD_KEY_P;
				m_ReverseKeyMap[0x51] = keyboard::PV_KEYBOARD_KEY_Q;
				m_ReverseKeyMap[0x52] = keyboard::PV_KEYBOARD_KEY_R;
				m_ReverseKeyMap[0x53] = keyboard::PV_KEYBOARD_KEY_S;
				m_ReverseKeyMap[0x54] = keyboard::PV_KEYBOARD_KEY_T;
				m_ReverseKeyMap[0x55] = keyboard::PV_KEYBOARD_KEY_U;
				m_ReverseKeyMap[0x56] = keyboard::PV_KEYBOARD_KEY_V;
				m_ReverseKeyMap[0x57] = keyboard::PV_KEYBOARD_KEY_W;
				m_ReverseKeyMap[0x58] = keyboard::PV_KEYBOARD_KEY_X;
				m_ReverseKeyMap[0x59] = keyboard::PV_KEYBOARD_KEY_Y;
				m_ReverseKeyMap[0x5A] = keyboard::PV_KEYBOARD_KEY_Z;

				m_ReverseKeyMap[VK_LEFT]	= keyboard::PV_KEYBOARD_KEY_LEFT;
				m_ReverseKeyMap[VK_RIGHT]	= keyboard::PV_KEYBOARD_KEY_RIGHT;
				m_ReverseKeyMap[VK_UP]		= keyboard::PV_KEYBOARD_KEY_UP;
				m_ReverseKeyMap[VK_DOWN]	= keyboard::PV_KEYBOARD_KEY_DOWN;
				m_ReverseKeyMap[VK_ESCAPE]	= keyboard::PV_KEYBOARD_KEY_ESCAPE;
				m_ReverseKeyMap[VK_TAB]		= keyboard::PV_KEYBOARD_KEY_TAB;
				m_ReverseKeyMap[VK_INSERT]	= keyboard::PV_KEYBOARD_KEY_INSERT;
				m_ReverseKeyMap[VK_HOME]	= keyboard::PV_KEYBOARD_KEY_HOME;
				m_ReverseKeyMap[VK_PRIOR]	= keyboard::PV_KEYBOARD_KEY_PAGEUP;
				m_ReverseKeyMap[VK_NEXT]	= keyboard::PV_KEYBOARD_KEY_PAGEDOWN;
				m_ReverseKeyMap[VK_END]		= keyboard::PV_KEYBOARD_KEY_END;
				m_ReverseKeyMap[VK_DELETE]	= keyboard::PV_KEYBOARD_KEY_DELETE;
				m_ReverseKeyMap[VK_BACK]	= keyboard::PV_KEYBOARD_KEY_BACKSPACE;
				m_ReverseKeyMap[VK_SPACE]	= keyboard::PV_KEYBOARD_KEY_SPACE;
				m_ReverseKeyMap[VK_RETURN]	= keyboard::PV_KEYBOARD_KEY_ENTER;
			}
			{
				m_CursorMap[CursorType::PV_ARROW]		= IDC_ARROW;
				m_CursorMap[CursorType::PV_TEXT_INPUT]	= IDC_IBEAM;
				m_CursorMap[CursorType::PV_RESIZE_ALL]	= IDC_SIZEALL;
				m_CursorMap[CursorType::PV_RESIZE_EW]	= IDC_SIZEWE; 
				m_CursorMap[CursorType::PV_RESIZE_NS]	= IDC_SIZENS; 
				m_CursorMap[CursorType::PV_RESIZE_NESW] = IDC_SIZENESW;
				m_CursorMap[CursorType::PV_RESIZE_NWSE] = IDC_SIZENWSE;
				m_CursorMap[CursorType::PV_HAND]		= IDC_HAND;
			}

			ChangeCursor(m_Data.cType);
		}

		WindowsWindow::~WindowsWindow() {
			ShutDown();
		}

		void WindowsWindow::OnUpdate() {
			while (PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			m_GraphicsAPI->Update();
		}

		void WindowsWindow::SetVSync(bool enabled) {
			m_Data.vSync = enabled;
			m_GraphicsAPI->SetVsync(enabled);
		}

		bool WindowsWindow::IsVSync() const {
			return m_Data.vSync;
		}

		void WindowsWindow::CreateOpenGLContext() {
			#ifdef PV_RENDERING_API_OPENGL
				m_GraphicsAPI = std::unique_ptr<GraphicsAPI>(new OpenGLAPI());
				m_GraphicsAPI->Init(hWnd, m_Data.width, m_Data.height);
				PV_ASSERT(m_GraphicsAPI->IsAPIReady(), "");
			#endif
		}

		bool WindowsWindow::IsKeyDown(int keyCode) {
			SHORT keyState = GetAsyncKeyState(m_KeyMap[keyCode]);
			if((1 << 15) & keyState) {
				return true;
			} else {
				return false;
			}
		}

		void WindowsWindow::ShutDown() {
			DestroyWindow(hWnd);
			m_GraphicsAPI->Delete();
			PostQuitMessage(0);
		}

		void WindowsWindow::ChangeCursor(CursorType type) {
			SetCursor(LoadCursor(NULL, m_CursorMap[type]));
			m_Data.cType = type;
		}

	}
}
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
					int keyCode = wParam;
					bool repeatCount = (lParam & 0x40000000);
					KeyPressedEvent event(keyCode, repeatCount);
					s_WindowPointer->m_Data.eventCallback(event);
				} catch(std::exception &e) {}
				break;
			}
			case WM_KEYUP:
			{
				try {
					int keyCode = wParam;
					KeyReleasedEvent event(keyCode);
					s_WindowPointer->m_Data.eventCallback(event);
				} catch(std::exception &e) {}
				break;
			}
			case WM_LBUTTONDOWN:
			{
				MouseButtonPressedEvent event(PV_MOUSE_BUTTON_LEFT);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_RBUTTONDOWN:
			{
				MouseButtonPressedEvent event(PV_MOUSE_BUTTON_RIGHT);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_MBUTTONDOWN:
			{
				MouseButtonPressedEvent event(PV_MOUSE_BUTTON_MIDDLE);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_XBUTTONDOWN:
			{
				unsigned short button = GET_XBUTTON_WPARAM(wParam);
				if(button == 1) {
					MouseButtonPressedEvent event(PV_MOUSE_BUTTON_THUMB1);
					s_WindowPointer->m_Data.eventCallback(event);
					break;
				} else if(button == 2) {
					MouseButtonPressedEvent event(PV_MOUSE_BUTTON_THUMB2);
					s_WindowPointer->m_Data.eventCallback(event);
					break;
				}
			}
			case WM_LBUTTONUP:
			{
				MouseButtonReleasedEvent event(PV_MOUSE_BUTTON_LEFT);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_RBUTTONUP:
			{
				MouseButtonReleasedEvent event(PV_MOUSE_BUTTON_RIGHT);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_MBUTTONUP:
			{
				MouseButtonReleasedEvent event(PV_MOUSE_BUTTON_MIDDLE);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_XBUTTONUP:
			{
				unsigned short button = GET_XBUTTON_WPARAM(wParam);
				if(button == 1) {
					MouseButtonReleasedEvent event(PV_MOUSE_BUTTON_THUMB1);
					s_WindowPointer->m_Data.eventCallback(event);
					break;
				} else if(button == 2) {
					MouseButtonReleasedEvent event(PV_MOUSE_BUTTON_THUMB2);
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
			SHORT keyState = GetKeyState(keyCode);
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
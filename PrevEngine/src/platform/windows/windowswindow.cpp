#include "pch.h"
#include "windowswindow.h"

#include "engine/events/applicationevent.h"
#include "engine/events/keyevent.h"
#include "engine/events/mouseevent.h"
#include "engine/assert.h"

#include "misc.h"
#include "API/DirectX/directxwindowsinitization.h"
#include "API/OpenGL/openglwindowsinitialization.h"

#include "essentials/input.h"

namespace prev {

	Window * Window::Create(const WindowProps &props) {
		return new windows::WindowsWindow(props);
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
			case WM_KEYDOWN:
			{
				bool repeatCount = (lParam & 0x40000000);
				KeyPressedEvent event(wParam, repeatCount);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_KEYUP:
			{
				KeyReleasedEvent event(wParam);
				s_WindowPointer->m_Data.eventCallback(event);
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
				MouseMovedEvent event(xPos, yPos);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			case WM_MOUSEWHEEL:
			{
				static float deltaScroll = 0.0f;
				float xPos = GET_WHEEL_DELTA_WPARAM(wParam);
				MouseScrolledEvent event(xPos, 0);
				s_WindowPointer->m_Data.eventCallback(event);
				break;
			}
			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
			}
			return 0;
		}

		//For std::string to std::wstring
		std::wstring s2ws(const std::string& s) {
			int len;
			int slength = (int)s.length() + 1;
			len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
			wchar_t* buf = new wchar_t[len];
			MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
			std::wstring r(buf);
			delete[] buf;
			return r;
		}

		WindowsWindow::WindowsWindow(const WindowProps &props) {
			PV_CORE_TRACE("Creating Windows window [Title = %s] [Width = %d] [Height = %d]", props.Title.c_str(), props.Width, props.Height);
			m_Data.title = props.Title;
			m_Data.width = props.Width;
			m_Data.height = props.Height;

			WNDCLASSEX wc;
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

			hWnd = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				s_WindowClassName,
				s2ws(props.Title).c_str(),
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, props.Width, props.Height,
				NULL, NULL, hInstance, NULL);

			if(hWnd == NULL) {
				PV_CORE_ERROR("Unable to create Windows Window");
				return;
			}

			ShowWindow(hWnd, SW_SHOWNORMAL);
			UpdateWindow(hWnd);
		}

		WindowsWindow::~WindowsWindow() {
			ShutDown();
		}

		void WindowsWindow::OnUpdate() {
			if(PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			m_GraphicsAPI->Update();
		}

		void WindowsWindow::SetVSync(bool enabled) {
			m_Data.vSync = enabled;
		}

		bool WindowsWindow::IsVSync() const {
			return m_Data.vSync;
		}

		void WindowsWindow::CreateOpenGLContext() 
		{
			m_GraphicsAPI = std::unique_ptr<GraphicsAPI>(new opengl::OpenGLAPI);
			m_GraphicsAPI->Init(hWnd);
			PV_ASSERT(m_GraphicsAPI->IsAPIReady(), "");
		}

		void WindowsWindow::CreateDirectXContext() {
			
		}

		void WindowsWindow::ShutDown() {
			wglDeleteContext(ourOpenGLRenderingContext);
			PostQuitMessage(0);
		}

	}
}
#include "pch.h"
#include "glfwwindow.h"

#include <libgen.h>

namespace prev {

	//Referenced from The Cherno Hazel Engine https://github.com/TheCherno/Hazel/

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char * description) {
		PV_CORE_ERROR("GLFW Error (%d): %s", error, description);
	}

	Window * Window::Create(const WindowProps & props) {
		return new GlfwWindow(props);
	}

	//for std::wstring to std::string
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
#ifdef PV_PLATFORM_WINDOWS
	std::string Window::GetExePath() {
		if (m_ExePath.size() != 0) {
			return m_ExePath;
		}
		HMODULE hModule = GetModuleHandle(NULL);
		WCHAR path[MAX_PATH];
		GetModuleFileName(hModule, path, MAX_PATH);
		std::string spath = ws2s(std::wstring(path));
		return spath.substr(0, spath.find_last_of("\\") + 1);
	}
#else
	std::string Window::GetExePath() {
		return "";
	}
#endif


	GlfwWindow::GlfwWindow(const WindowProps & props) {
		PV_CORE_TRACE("Creating GLFW window [Title = %s] [Width = %d] [Height = %d]", props.Title.c_str(), props.Width, props.Height);
		m_Data.title = props.Title;
		m_Data.width = props.Width;
		m_Data.height = props.Height;

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			PV_ASSERT(success, "Could not intialize GLFW!");
			//glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow * window, int width, int height) {
			WindowData & data = *(WindowData *)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.eventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow * window) {
			WindowData & data = *(WindowData *)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.eventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow * window, int key, int scancode, int action, int mods) {
			WindowData & data = *(WindowData *)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.eventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.eventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow * window, unsigned int keycode) {
			WindowData & data = *(WindowData *)glfwGetWindowUserPointer(window);

			CharacterEvent event(keycode);
			data.eventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow * window, int button, int action, int mods) {
			WindowData & data = *(WindowData *)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.eventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow * window, double xOffset, double yOffset) {
			WindowData & data = *(WindowData *)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.eventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow * window, double xPos, double yPos) {
			WindowData & data = *(WindowData *)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.eventCallback(event);
		});
	}

	GlfwWindow::~GlfwWindow() {
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void GlfwWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	unsigned int GlfwWindow::GetWidth() const {
		return m_Data.width;
	}

	unsigned int GlfwWindow::GetHeight() const {
		return m_Data.height;
	}

	void GlfwWindow::SetEventCallback(const EventCallbackFn & callback) {
		m_Data.eventCallback = callback;
	}

	void GlfwWindow::SetVSync(bool enabled) {
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.vSync = enabled;
	}

	bool GlfwWindow::IsVSync() const {
		return m_Data.vSync;
	}

	bool GlfwWindow::IsKeyDown(int keyCode) {
		return glfwGetKey(m_Window, keyCode);
	}

	void GlfwWindow::CreateOpenGLContext() {
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PV_ASSERT(status, "Failed to initialize Glad!");
	}

	void GlfwWindow::ChangeCursor(CursorType type) {
	}

	void * GlfwWindow::GetRawWindow() {
		return m_Window;
	}

}
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/window.h"
#include "essentials/input.h"

#include "engine/events/event.h"
#include "engine/events/applicationevent.h"
#include "engine/events/keyevent.h"
#include "engine/events/mouseevent.h"

namespace prev {

	class GlfwWindow : public Window {
		friend class Input;
	public:
		GlfwWindow(const WindowProps & props);
		virtual ~GlfwWindow();
	private:
		struct WindowData {
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn eventCallback = nullptr;
		};
		WindowData m_Data;
		GLFWwindow * m_Window;
	private:
		// Inherited via Window
		virtual void OnUpdate() override;
		virtual unsigned int GetWidth() const override;
		virtual unsigned int GetHeight() const override;
		virtual void SetEventCallback(const EventCallbackFn & callback) override;
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;
		virtual bool IsKeyDown(int keyCode) override;
		virtual void CreateOpenGLContext() override;
		virtual void ChangeCursor(CursorType type) override;
		virtual void * GetRawWindow() override;
	};

}